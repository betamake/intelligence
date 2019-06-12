#include "idcardthread.h"
#include "Serial.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "Instructions.h"
#include "ServerSocket.h"
#include <pthread.h>
#include <QDebug>
#include <string>

/**
 * @brief IdCardThread::IdCardThread
 * @param parent
 * 子线程读取数据
 */
IdCardThread::IdCardThread(QObject *parent) :QThread(parent)
{
    //    isChecked = false;
}

//终止线程
IdCardThread::~IdCardThread() {
    // 请求终止
}
void IdCardThread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}
void IdCardThread::run()
{
    m_isCanRun = true;//标记可以运行
    Serial serial;//串口
    //    ServerSocket serverSocket;//socket server
    Instructions instructions;//指令处理
    Information information;//存储解析后的身份证信息
    char buf[1024];//指令存储
    int len;//返回信息长度
    int findFlag = -1,selectFlag = -1,readBasicFlag = -1;//找卡 选卡 读基本信息是否成功
    int count=0;//
    //    pid_t childProcess;
    //    int sendFlag;
    /**
       * 打开串口
       */
    if(serial.open("/dev/ttyUSB0",115200,8,Serial::PARITY_NONE,1)!=Serial::OK)
    {
        qDebug()<<"Cannot open serial port!\n";
        emit emitIdInfo (information, 1);
        return;
    }else
    {
        // 三种基本指令
        char* command_find = reinterpret_cast<char *>(instructions.find);
        char* command_select =reinterpret_cast<char *>(instructions.select);
        char* command_read_basic = reinterpret_cast<char *>(instructions.readBasic);

        while(count < 15) {
            count++;
            qDebug()<<"====recognized times:  ===="<< count<<"\n";

            QMutexLocker locker(&m_lock);
            if(!m_isCanRun)//在每次循环判断是否可以运行，如果不行就退出循环
            {
                qDebug()<<"==== 关闭串口:  ===="<< findFlag<<"\n";
                serial.close();
                return;
            }

            serial.write(command_find, COMMAND_LENGTH);
            len = serial.read(buf, sizeof(buf), 100);
            buf[COMMAND_LENGTH + 1] = 0;
            findFlag = instructions.findCardSuccessfully(buf);
            qDebug()<<"==== 1.找卡是否成功:  ===="<< findFlag<<"\n";
            if(findFlag == 1) {
                serial.write(command_select, COMMAND_LENGTH);
                len = serial.read(buf, sizeof(buf), 100);
                buf[COMMAND_LENGTH + 1] = 0;
                selectFlag = instructions.selectCardSuccessfully(buf);
                qDebug()<<"==== 2.选卡是否成功: ===="<< selectFlag<<"\n";
                if(selectFlag == 1) {
                    serial.write(command_read_basic, COMMAND_LENGTH);
                    len = serial.read(buf, sizeof(buf), 500);
                    buf[len] = 0;
                    readBasicFlag = instructions.readBasicSuccessfully(buf);
                    qDebug()<<"==== 3.读基本信息是否成功:  ===="<< readBasicFlag<<"\n";
                    qDebug()<<"==== 3.读无指纹基本信息 读取字节数: ===="<< len<<"\n";
                    if(readBasicFlag == 1 && len>0)
                    {//找卡失败错误码:000080  找卡不成功
                        instructions.parseInformation(buf);
                        information = instructions.getInformation();
//                        cout << information << endl;

                        //读卡成功 发射信号
                        idInfo = information.toString();
                        qDebug() << idInfo<<endl;
                        qDebug()<<"=============================="<<"\n\n";
                        emit emitIdInfo(information, 0);
                        return;
                    }
                }
            }
            sleep(1);
        };

        qDebug()<<"count:"<< count;
        emit emitIdInfo (information, 2);
    }
}
