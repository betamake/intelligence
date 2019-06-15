
//#include "winnt.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commonutils.h"

#include <QAudioProbe>
#include <QAudioRecorder>
#include <QMediaRecorder>
#include<QMediaPlayer>
#include<QCameraInfo>
#include<QCameraViewfinder>
#include<QCameraImageCapture>
#include<QCoreApplication>
#include<QFileDialog>
#include<QDebug>
#include <unistd.h>

#include <turing.h>

#include<QWidget>
#include <QQuickView>
#include <QTimer>
#include<QByteArray>
#include<QtNetwork>
#include<QMap>
#include<QFileDialog>
#include<QJsonDocument>
#include<QJsonParseError>
#include<QScriptEngine>
#include<QScriptValue>
#include<QScriptValueIterator>
#include<QStandardItemModel>
#include<QMessageBox>
#include <QtGlobal>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDir>
#include <QString>
#include <QTime>
#include <QStringList>
#include <QThread>
#include <QDialog>
#include<QPainter>
#include<QRect>
#include<QVariant>
#include <QProcess>

static qreal getPeakValue(const QAudioFormat &format);
static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffr, int frames, int channels);


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),m_currentRunLoaclThread(NULL),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    currentIndex = 10;
    //ui->mainWidget->resize (800,800);
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->faceManBtn->hide();//人脸库管理登陆后才展示
    ui->lastStepBtn->hide ();
    ui->firstBtn->hide ();
    this->noShowguiInforn ();

    ui->userInfo->installEventFilter(this); //初始化个人信息页
    //    address = "http://192.168.15.58:8000";
    address = "http://192.168.3.185:8000";
    //        address = "http://211.71.15.58:8000";
    qDebug() << "length:"<< address.length();

    regCode = 0;//初始化注册信息
    loginStatus = 0;//未登录
    idCheckId = 0;//初始化身份证调用id

    isScan=false;

    qRegisterMetaType<Information>("Information");//注册information类
    qRegisterMetaType<QMap<QString,QString>>("QMap<QString,QString>");//注册information类
    qRegisterMetaType<QByteArray>("QByteArray");//注册QByteArray类

    expenseType = 11;//报销类型页面号   //这两行没有用 @hkl
    expenseTypeId = 11;//判断提交数据的前一页
    busiTalble = false;  //这一行是否需要,需检查@hkl
    this->initMedia();
    //处理表格信息
    //connect (this,&MainWindow::costDone,this,&MainWindow::dealCost); //发票识别完成信息之后,处理识别发票信息;
    connect (this,&MainWindow::busiDone,this,&MainWindow::dealBusi); //车票识别完成信息之后,处理识别车票信息;
    connect (this,&MainWindow::abroadDone,this,&MainWindow::dealAbroad);
}

//QString MainWindow::HOST =  "http://192.168.3.102:8000";

MainWindow::~MainWindow()
{
    qDebug() << "start destroy widget";
    //    m_thread->stopImmediately();
    //    //局部线程的终结
    //    if(m_currentRunLoaclThread)
    //    {
    //        m_currentRunLoaclThread->stopImmediately();
    //    }
    //    m_thread->wait(i);
    delete ui;

    //    if(m_objThread)i
    //    {
    //        m_objThread->quit();
    //        //        m_obj->stop();
    //    }
    //    m_objThread->wait();
    qDebug() << "end destroy widget";
}

//初始化多媒体
void MainWindow::initMedia()
{
    //照相机配置
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if(cameras.count() > 0){
        foreach (const QCameraInfo &cameraInfo, cameras) {
            qDebug()<<cameraInfo.description();
        }
        camera = new QCamera(cameras.at(0));
    }

    //语音识别设置
    audioRecorder = new QAudioRecorder(this);
    probe = new QAudioProbe;

    connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)),this, SLOT(processBuffer(QAudioBuffer)));
    probe->setSource(audioRecorder);
    recordNum = 0;
    recordHigh = 0;
    recordLow = 0;
    recordSend = false;
    this->setRecord();
    this->startRecord();

    //初始化音频播放变量
    player = new QMediaPlayer;
    playStatus = false;
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(playChanged()));



    //    ui->mainWidget->setWindowFlags(Qt::Window);
    //    ui->mainWidget->showFullScreen();
    player->stop();
    this->sendPlayText("欢迎使用财务机器人");
}

//-------------------------------------------event start----------------------------------------
//首页登录方式选择
void MainWindow::on_LogBtn_clicked()
{
    player->stop();
    this->sendPlayText("请选择登录方式");
    currentIndex =2;
    ui->mainWidget->setCurrentIndex(currentIndex);
}


/**
* @brief      登录方式选择人脸
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::on_faceBtn_clicked()
{
    player->stop();
    this->sendPlayText("已选择人脸登录");

    // jump to facecheck page
    currentIndex = 3;
    ui->mainWidget->setCurrentIndex(currentIndex);
    isFaceOk = false;
    //扫描人脸次数
    facetime = 0;

    // 设置取景器
    //    camera = new QCamera(this);
    viewfinder = new QCameraViewfinder(this);
    ui->imageLayout->addWidget(viewfinder);
    camera->setViewfinder(viewfinder);
    viewfinder->resize(600,350);
    //    viewfinder->show();

    imageCapture = new QCameraImageCapture(camera);
    camera->setCaptureMode(QCamera::CaptureStillImage);
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    camera->start();

    timer = new QTimer(this);
    timer->setInterval(5000);//1s发送一次timeout信号
    timer->start();//启动定时器
    connect(timer, SIGNAL(timeout()), this, SLOT(faceCheck()));
}



/**
* @brief        身份证登录
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::on_idBtn_clicked()
{
    player->stop();
    this->sendPlayText("请将身份证放在指定位置");
    currentIndex =5;
    //进去之前先隐藏控件
    ui->idUsername->setAlignment(Qt::AlignCenter);
    ui->idUsername->setText(tr("正在识别中...."));
    ui->idLabel->hide();
    ui->idLabel2->hide();
    ui->idLabel3->hide();
    ui->idLabel4->hide();
    ui->idLabel5->hide();

    //隐藏显示框
    ui->idName->hide();
    ui->idAddress->hide();
    ui->idCardNumber->hide();
    ui->idPhone->hide();
    ui->idSex->hide();

    idCheckId = 2;
    isIdcardChecked = false;
    ui->mainWidget->setCurrentIndex(currentIndex);
    this->idCardRead();
}
/**
* @brief         身份证登录查询数据库
* @author        胡帅成
* @date        2018-09-08
*/
void MainWindow::on_idLogin_clicked()
{
    //走到这里必然登录成功了
    if(isIdcardChecked)
    {
        player->stop();
        this->sendPlayText("身份证登录成功");
        ui->lastStepBtn->show ();
        ui->nextStepBtn->show ();
        currentIndex = 9;
        ui->mainWidget->setCurrentIndex(currentIndex);//登录成功跳转到欢迎页
        //登录完成展示用户信息
        loginStatus = 2;
        this->showUserInfo(loginUser);

    }else
    {
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("请将身份证放到指定位置!"));
        player->stop();
        this->sendPlayText("请将身份证放到指定位置");
    }

}

/**
* @brief        选择账号
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::on_accountBtn_clicked()
{
    player->stop();
    currentIndex = 4;
    this->sendPlayText("已选择账号登录");
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->lastStepBtn->show ();
}

/**
* @brief         账号登录查询数据库
* @author        胡想成
* @date          2018-请选择登录方式请选择登录方式08-15
*/
void MainWindow::on_user_loginBtn_clicked()
{
    ui->lastStepBtn->show ();//显示上一步按钮
    //连接数据库
    database.outputUserInfo();
    //判断用户名和密码是否正确
    QString username = ui->user_loginEdit->text().trimmed();
    QString password = ui->user_pwdEdit->text().trimmed();
    if(!(username.isEmpty() && password.isEmpty()))
    {
        if(database.verificationLogin(username, password))
        {
            qDebug() << "验证结果："<< database.verificationLogin(username, password);
            //登录成功
            QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("登录成功！"));
            //查数据库
            loginStatus = 3;//账号登录状态
            loginUser = database.SearchUserByUsername(username);
            this->showUserInfo(loginUser);

            player->stop();
            this->sendPlayText("账号登录成功");

            //            currentIndex = 9;
            //            this->toNextStep();
            currentIndex = 9;
            ui->mainWidget->setCurrentIndex (currentIndex);
            ui->nextStepBtn->show();
            ui->lastStepBtn->show ();

            //下次进来的时候
            ui->user_loginEdit->clear();
            ui->user_pwdEdit->clear();
            ui->user_loginEdit->setFocus();
        }
        else
        {
            //登录失败，清空用户编辑框，密码编辑框，设置光标到用户编辑框
            QMessageBox::information(this, QString::fromUtf8("失败"),QString::fromUtf8("登录失败！"));
            player->stop();
            this->sendPlayText("账号有误，请重新登录");
            //清空内容并定位光标
            ui->user_loginEdit->clear();
            ui->user_pwdEdit->clear();
            ui->user_loginEdit->setFocus();
        }
    }else
    {
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("用户名密码不能为空！"));
        player->stop();
        this->sendPlayText("用户名密码不能为空");
    }
}

//首页注册方式选择
void MainWindow::on_RegBtn_2_clicked()
{
    currentIndex = 6;
    ui->mainWidget->setCurrentIndex(currentIndex);
}

/**
* @brief       人脸注册拍照
* @author        胡帅成
* @date        2018-09-06
*/
//void MainWindow::on_faceRegBtn_clicked()
//{

//    //设置取景器
//    viewfinder = new QCameraViewfinder(this);
//    ui->faceRegImageLayout->addWidget(viewfinder);
//    camera->setViewfinder(viewfinder);
//    viewfinder->resize(600,350);

//    imageCapture2 = new QCameraImageCapture(camera);
//    camera->setCaptureMode(QCamera::CaptureStillImage);
//    imageCapture2->setCaptureDestination(QCameraImageCapture::CaptureToFile);

//    //注册时ID为1
//    idFace=1;
//    ui->mainWidget->setCurrentIndex(6);
//    camera->searchAndLock();
//    currentIndex = 0;

//    camera->start();
//    camera->unlock();
//}


/**
* @function   on_carmBtn_clicked
* @brief      人脸注册插入人脸库
* @author        胡想成
* @date          2018-08-15
*/
void MainWindow::on_carmBtn_clicked()
{
    //    qDebug()<<camera->status()<<endl;

    idFace=1;
    camera->searchAndLock();
    //    imageCapture2->capture();
    imageCaptureReg->capture();
    camera->unlock();
    connect(imageCaptureReg, SIGNAL(imageCaptured(int,QImage)), this, SLOT(photoRegister(int,QImage)));
}


/**
* @brief       账户注册插入数据库
* @author        胡想成
* @date          2018-08-15
*/
void MainWindow::on_RegAcountBtn_clicked()
{
    //获取字段值，封装user对象
    QString username = ui->RegUsername_LineEdit->text().trimmed();
    QString password = ui->RegPwd_LineEdit->text().trimmed();
    QString phone = ui->RegPhoneNum_LineEdit->text().trimmed();
    /*@author:胡坤伦
     *@brief:删除avaterLineEdit
     *@date:2019-02-28
     */
    //QString avaterPath = ui->avaterLineEdit->text().trimmed();

    if(!(username.isEmpty() || password.isEmpty() || phone.isEmpty()))
    {
        //数据库查询一下此用户名是否存在
        if(database.checkUsername(username))
        {
            regCode = 0;
            QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("用户名已被注册！"));
        }
        else
        {
            //封装数据
            QUuid id = QUuid::createUuid();
            QString uid = id.toString();
            uid.remove("{").remove("}").remove("-"); // 一般习惯去掉左右花括号和连字符
            qDebug() << "uid:"<< uid;

            User user(uid,regUser.getAvater(),"",username,password,"","",phone,commonutils.getCurrentTime(),commonutils.getCurrentTime());

            if(database.insertUser(user))
            {
                regCode = 1;//已进入账号注册状态
                regUser = user; //authoid存在乱码问题
                //                regUser.setAuthoId (0);
                player->stop();
                this->sendPlayText("账户注册成功");
                QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("注册成功！"));

                //clear
                ui->RegUsername_LineEdit->clear ();
                ui->RegPwd_LineEdit->clear ();
                ui->RegPhoneNum_LineEdit->clear ();
                ui->avaterLineEdit->clear ();
                ui->avaterRegBoxLabel->clear();

                //                this->on_facePreBtn_clicked();
                toCurrentPage (6);//init carmera

            }else
            {
                player->stop();
                this->sendPlayText("帐号注册失败");
                QMessageBox::information(this, QString::fromUtf8("失败"),QString::fromUtf8("注册失败!"));

                currentIndex = 6;
                ui->mainWidget->setCurrentIndex(currentIndex);
                //clear
                ui->RegUsername_LineEdit->clear ();
                ui->RegPwd_LineEdit->clear ();
                ui->RegPhoneNum_LineEdit->clear ();
                ui->avaterLineEdit->clear ();
                ui->avaterRegBoxLabel->clear();

            }
        }
    }
    else
    {
        player->stop();
        this->sendPlayText("字段不能为空");
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("字段不能为空！"));
    }
}


/**
* @brief      发送音频文本，进行语音合成
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::sendPlayText(QByteArray text){
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(playReply(QNetworkReply*)));

    QUrlQuery params;

    text = text.toBase64();
    text = text.replace("+","-");
    text = text.replace("/","_");
    params.addQueryItem("text",text);
    QString  data = params.toString();

    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/understand/synthesis/"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->post(request,params.toString().toUtf8());
}


/**
* @brief         票据识别事件
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::on_fileChooseBtn_clicked()
{

    player->stop();
    this->sendPlayText("请选择票据");
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("打开图片"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg)"));
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(fileDialog->exec()){
        fileNames = fileDialog->selectedFiles();
    }
    QString fileName;
    fileName = fileNames.at(0);
    billpath = fileName;
    qDebug()<<fileName<<endl;
    manager = new QNetworkAccessManager(this);
    //读取完成后识别票据
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(billReply(QNetworkReply*)));

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)||file.size()==0)
    {
        file.close();
        return ;
    }
    QByteArray fdata = file.readAll();
    file.close();
    fdata = fdata.toBase64();
    fdata = fdata.replace("+","-");
    fdata = fdata.replace("/","_");

    QUrlQuery params;
    params.addQueryItem("image",fdata);
    params.addQueryItem("imageType",".jpg");
    QString  data = params.toString();

    //打印票据图片编码
    this->printFile("billReply.txt", fdata);
    //    QString address = HOST.append("/bill/");

    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/bill/"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->post(request,params.toString().toUtf8());
}

/**
* @brief        确认并提交
* @author
* @date        2018-09-06
*/
void MainWindow::on_confirmBtn_clicked()
{
    player->stop();
    this->sendPlayText("确认提交");
    //差旅报销单数据插入
    if(expenseType == 12)
    {

        QString bexp[18];
        User usr;
        qDebug()<<"uname:"<<uname;
        usr = database.SearchUserByUsername(uname);
        qDebug()<<"usr.getUid():"<<usr.getUid();
        QString uid = usr.getUid();//10进制将int转换为16进制
        qDebug()<<uid;
        busiexp.setBusimanid(uid);
        exphead.setHeadoperatori(uid);
        qDebug()<<"bsTable->rowCount():"<<bsTable->rowCount();
        for (int i=0; i< bsTable->rowCount(); i++)
        {
            for (int j=0;j< bsTable->columnCount();j++)
            {
                if(bsTable->item(i,j)!=NULL)
                {
                    bexp[j] =  bsTable->item(i,j)->text();
                }
                else {
                    bexp[j]="";
                }
            }
            //headers<<"出差人"<<"起始时间"<<"终止时间"<<"出差天数"<<"出发地点"<<"到达地点"<<"市间交通费"<<"交通费单据张数"
            //<<"住宿费用"<<"住宿单据张数"<<"补贴天数"<<"交通补贴"<<"伙食补贴"<<"其他费用金额"<<"其他费用单据张数"<<"其他费用说明"<<"小计";

            //busiexp.setBusimanid(bexp[0]);
            busiexp.setbusid(bexp[0]);
            pay.setPayInfoBusid(bexp[0]);
            exphead.setHeadbusid(bexp[0]);
            busiexp.setBusibegindate(bexp[2]);
            busiexp.setBusienddate(bexp[3]);
            busiexp.setBusiday(bexp[4]);
            busiexp.setBusifromadres(bexp[5]);
            busiexp.setBusitoadres(bexp[6]);
            busiexp.setBusitranscost(bexp[7]);
            busiexp.setBusitransbillnum(bexp[8]);
            busiexp.setBusiaccomcost(bexp[9]);
            busiexp.setbusiAccombillnum(bexp[10]);
            busiexp.setBusirelaxday(bexp[11]);
            busiexp.setBusitranssubsidy(bexp[12]);
            busiexp.setBusifoodsubsidy(bexp[13]);
            busiexp.setBusiothercost(bexp[14]);
            busiexp.setBusiothernum(bexp[15]);
            busiexp.setBusiothermemo(bexp[16]);
            busiexp.setBusitotalcost(bexp[17]);

            qDebug()<<"busiexp:"<<busiexp.getBusimanid()<<busiexp.getBusibegindate();
            database.insertBusiexp(busiexp);

            busiexp.setEmpty ();

        }
    }
    //费用报销单数据插入
    else if (expenseType == 13)
    {
        QString bexp[9];
        User usr;
        qDebug()<<"uname:"<<uname;
        usr = database.SearchUserByUsername(uname);
        qDebug()<<"usr.getUid():"<<usr.getUid();
        QString uid = usr.getUid();//10进制将int转换为16进制
        qDebug()<<uid;
        costexp.setCostmanid(uid);
        exphead.setHeadoperatori(uid);
        qDebug()<<"costTable->rowCount():"<<costTable->rowCount();
        for (int i=0; i< costTable->rowCount(); i++)
        {
            for (int j=0;j< costTable->columnCount();j++)
            {
                if(costTable->item(i,j)!=NULL)
                {
                    bexp[j] =  costTable->item(i,j)->text();
                }
                else {
                    bexp[j]="";
                }
            }
            //headers<<"报销单号"<<"报销人"<<"报销金额"<<"单据张数"<<"预算项目"<<"预算归属部门"<<"借款单"<<"剩余借款"<<"备注";
            //busiexp.setBusimanid(bexp[0]);
            costexp.setcostid(bexp[0]);
            pay.setPayInfoCostid(bexp[0]);
            exphead.setHeadcostid(bexp[0]);
            costexp.setCostmoney(bexp[2]);
            costexp.setCostbillamount(bexp[3]);
            costexp.setCostbudgtproid(bexp[4]);
            costexp.setCostcentrializedid(bexp[5]);
            costexp.setCostloanid(bexp[6]);
            costexp.setCostsurplusloan(bexp[7]);
            costexp.setCostmemo(bexp[8]);

            database.insertCostexp(costexp);
        }
    }
    //出国报销单数据插入
    else if (expenseType == 14)
    {
        //if()

        QString bexp[24];
        User usr;
        qDebug()<<"uname:"<<uname;
        usr = database.SearchUserByUsername(uname);
        qDebug()<<"usr.getUid():"<<usr.getUid();
        QString uid = usr.getUid();//10进制将int转换为16进制
        qDebug()<<uid;
        busiexp.setBusimanid(uid);
        exphead.setHeadoperatori(uid);
        qDebug()<<"abdTable->rowCount():"<<abdTable->rowCount();
        for (int i=0; i< abdTable->rowCount(); i++)
        {
            for (int j=0;j< abdTable->columnCount();j++)
            {
                if(abdTable->item(i,j)!=NULL)
                {
                    bexp[j] =  abdTable->item(i,j)->text();
                }
                else {
                    bexp[j]="";
                }
            }
            //            headers<<"报销单编号"<<"出差人"<<"起始时间"<<"终止时间"<<"出差天数"<<"出发地点"<<"到达地点"<<"市间交通费"<<"交通费单据张数"
            //                  <<"住宿费用"<<"住宿单据张数"<<"补贴天数"<<"交通补贴"<<"伙食补贴"<<"签证费金额"<<"签证费单据张数"<<"服务费金额"
            //                 <<"服务费票据张数"<<"出国换汇金额"<<"出国换汇金额票据张数"<<"其他费用金额"<<"其他费用单据张数"<<"其他费用说明"<<"小计";
            //busiexp.setBusimanid(bexp[0]);
            busiexp.setAbdid(bexp[0]);
            busiexp.setBusibegindate(bexp[2]);
            busiexp.setBusienddate(bexp[3]);
            busiexp.setBusiday(bexp[4]);
            busiexp.setBusifromadres(bexp[5]);
            busiexp.setBusitoadres(bexp[6]);
            busiexp.setBusitranscost(bexp[7]);
            busiexp.setBusitransbillnum(bexp[8]);
            busiexp.setBusiaccomcost(bexp[9]);
            busiexp.setbusiAccombillnum(bexp[10]);
            busiexp.setBusirelaxday(bexp[11]);
            busiexp.setBusitranssubsidy(bexp[12]);
            busiexp.setBusifoodsubsidy(bexp[13]);
            busiexp.setBusiothercost(bexp[20]);
            busiexp.setBusiothernum(bexp[21]);
            busiexp.setBusiothermemo(bexp[22]);
            busiexp.setBusitotalcost(bexp[23]);

            abdexp.setAbdid(bexp[0]);
            abdexp.setAbdvisacost(bexp[14]);
            abdexp.setAbdvisabillnum(bexp[15]);
            abdexp.setAbdservcost(bexp[16]);
            abdexp.setAbdservbillnum(bexp[17]);
            abdexp.setAbdexchgcost(bexp[18]);
            abdexp.setAbdexchgbillnum(bexp[19]);

            //qDebug()<<"busiexp:"<<busiexp.getBusimanid()<<busiexp.getBusibegindate();
            database.insertBusiexp(busiexp);
            database.insertAbdexp(abdexp);
            busiexp.setEmpty ();
        }

    }
    database.insertExpensehead(exphead);
    database.insertPayinfo(pay);
    pay.setEmpty ();


    blchkTable = ui->billTable;
    if (bldir.size())
    {
        QString bexp[8];
        User usr;
        qDebug()<<"uname:"<<uname;
        usr = database.SearchUserByUsername(uname);
        qDebug()<<"usr.getUid():"<<usr.getUid();
        QString uid = usr.getUid();//10进制将int转换为16进制
        qDebug()<<uid;

        qDebug()<<"blchkTable->rowCount():"<<blchkTable->rowCount();
        for (int i=0; i< blchkTable->rowCount(); i++)
        {
            for (int j=0;j< blchkTable->columnCount();j++)
            {
                if(blchkTable->item(i,j)!=NULL)
                {
                    bexp[j] =  blchkTable->item(i,j)->text();
                }
                else {
                    bexp[j]="";
                }
            }
            billinfo.setBilltype(bexp[0]);
            billinfo.setBillcontent(bexp[1]);
            billinfo.setBilldate(bexp[2]);
            billinfo.setBillmoney(bexp[4]);
            billinfo.setBillstartplace(bexp[5]);
            billinfo.setBillendplace(bexp[6]);
            billinfo.setBillother(bexp[7]);
            billinfo.setBillrealpath(billdir);
            billinfo.setBillattachmenttitle(bldir.at(i));
            billinfo.setBilluser(uid);
            database.insertBillcheck(billinfo);
            //clear
            billinfo.setEmpty ();
        }
    }
    //保存票据电子版
    //    QImage img(billpath);
    //    QPixmap pixmap = QPixmap::fromImage(img);
    //    pixmap.save(billdir);
    //    busi.init();

    //           QLineEdit * busiCause1 = ui->busiCause_lineEdit;
    //           QLineEdit * busiForm1 = ui->busiForm_lineEdit;
    //           QLineEdit * busiProject1 = ui->busiProject_lineEdit;
    //    ui->busiDept_lineEdit
    //    ui->busiCause_lineEdit;
    //    ui->busiForm_lineEdit;
    //     ui->busiProject_lineEdit;

    player->stop();
    this->sendPlayText("报销成功");
    currentIndex = 17;
    ui->mainWidget->setCurrentIndex(currentIndex);
}


//-------------------------------------------event end----------------------------------------


//-------------------------------------------sequence action start----------------------------------------
//上一步
void MainWindow::toLastStep(){
    //报销类型的上一步是页面
    if(currentIndex == 12 || currentIndex == 13 || currentIndex == 14){
        currentIndex = 11;
    }else if(currentIndex == 15){
        currentIndex = expenseType;
    }
    else if (currentIndex==11)//身份认证的上一页是
    {
        currentIndex=10;
    }
    else if (currentIndex==10)//身份认证
    {
        currentIndex=9;
    }
    else if (currentIndex==6)//人脸注册
    {
        currentIndex=5;
    }
    else if (currentIndex==5)//身份认证的上一步是人脸注册
    {
        currentIndex=2;
    }
    else if (currentIndex==3 || currentIndex==4 ||currentIndex==5)//登录方式
    {
        currentIndex=2;
    }
    else if (currentIndex==2)
    {
        currentIndex=1;
    }
    else if(currentIndex > 0 && currentIndex != 9){
        currentIndex -- ;
    }
    ui->mainWidget->setCurrentIndex(currentIndex);
}
/*
 * currentIndex：
   0:welcomepage  //三个模块选择
   1:startpage  用户设置  登录还是注册// 显示上一步
   2:loginpage  登录方式 // 显示上一步

   3:face 人脸登录  //
   4:acount 账户登录  //上一步
   5:idCord 身份证登录  //上一步

   6:accountReg:账号注册 //上一步
   7:faceReg 人脸注册 //上一步
   8:RegChoose 身份证认证 //上一步

   9:welcomePage:欢迎页 //上一步 下一步
   10:bill 票据录入 //上下

   11:expenseChoice 报销类别 //上一步
   12:busiExpense  差旅报销单 //上下
   13:costExpense 费用报销单 //上下
   14:abroadExpense 出国报销单 //上下

   15:payInfo  支付信息 // 上下
   16:confirm 信息确认 // 上一步
   17:finish 报销完成
*/
//下一步
void MainWindow::toNextStep(){
    //选择身份验证的下一步是3
    if(currentIndex == 3 || currentIndex == 4 || currentIndex == 5){//在人脸识别或用户名密码登陆或身份证识别页面
        currentIndex = 9;//跳转到选择报销类别页面
    }else if(currentIndex == 7 || currentIndex == 8) {
        ui->nextStepBtn->hide();
    }
    else if(currentIndex == 6){//注册成功的下一页是登录页1
        currentIndex ++;
    }else if(currentIndex == 9){//欢迎页
        currentIndex =10;
        ui->lastStepBtn->hide ();
        ui->nextStepBtn->hide ();
    }
    else if(currentIndex ==10)
    {
        ui->nextStepBtn->show ();
        ui->lastStepBtn->show ();
        currentIndex =  expenseType; //当在选择报销类型时,应该注意到
        currentIndex =  11;

    }
    else if(currentIndex ==11)
    {
        ui->nextStepBtn->show ();
        ui->lastStepBtn->show ();
        //录入票据之后需要点击下一步需要进行判定  结合标志位
        currentIndex=13; //在这三种报销中,发票肯定会有.所以先跳转到发票报销页面.

    }
    else if(currentIndex == 12){//在报销页面的下一步是支付页
        expenseType = 12;
        //获取输入值，封装成busi对
        QLineEdit * busiDept1 = ui->busiDept_lineEdit;
        QLineEdit * busiCause1 = ui->busiCause_lineEdit;
        QLineEdit * busiForm1 = ui->busiForm_lineEdit;
        QLineEdit * busiProject1 = ui->busiProject_lineEdit;
        if(busiDept1->text().isEmpty()||busiCause1->text().isEmpty()||busiForm1->text().isEmpty()
                ||busiProject1->text().isEmpty())
        {
            QMessageBox::warning(this, tr("警告"),tr("请输入数据"),QMessageBox::Yes);
            currentIndex = 12;
        }
        else
        {
            qDebug()<<tr("busi不为空");
            //表头数据
            exphead.setHeaddeptid(busiDept1->text().trimmed());
            exphead.setHeadpurpose(busiCause1->text().trimmed());
            exphead.setHeadprojectid(busiProject1->text().trimmed());
            exphead.setHeadrequisitionid(busiForm1->text().trimmed());

            busi.setBusiCause_line(busiCause1->text().trimmed());
            busi.setBusiDept_line(busiDept1->text().trimmed());
            busi.setBusiForm_line(busiForm1->text().trimmed());
            busi.setBusiProject_line(busiProject1->text().trimmed());

            busi.toString();
            expenseTypeId = 12;
            if (abroadType == true)//这里需要做判断 根据录入是否有出国飞机票报销,在确定是否直接跳转到支付页面
            {
                currentIndex = 14;
            }
            else
            {
                currentIndex = 15;
            }

        }

        //确认界面显示出差报销单数据
        confirmTable = ui->confirmExcep_Table;
        confirmTable->setColumnCount(18);//设置列数
        confirmTable->setRowCount(bsTable->rowCount());
        QStringList headers;
        headers<<"报销单号"<<"出差人"<<"起始时间"<<"终止时间"<<"出差天数"<<"出发地点"<<"到达地点"<<"市间交通费"<<"交通费单据张数"<<"住宿费用"<<"住宿单据张数"<<"补贴天数"<<"交通补贴"<<"伙食补贴"<<"其他费用金额"<<"其他费用单据张数"<<"其他费用说明"<<"小计";
        confirmTable->setHorizontalHeaderLabels(headers);
        confirmTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        for ( int i=0;i<bsTable->rowCount();i++)
        {
            for (int j=0;j<bsTable->columnCount();j++)
            {
                if(bsTable->item(i,j)!=NULL)
                {
                    //                    bsTable=busTable->item(i,j)->text();
                    confirmTable->setItem(i,j,new QTableWidgetItem(bsTable->item(i,j)->text()));
                }
            }
        }

    }else if(currentIndex == 13)
    {
//        //获取输入值，封装成busi对象
////        QComboBox * costType1 = ui->costType_comboBox;
////        QLineEdit * costDept1 = ui->costDept_lineEdit;
////        QLineEdit * costContra1 = ui->costContra_lineEdit;
////        QLineEdit * costContraNum1 = ui->costContraNum_lineEdit;
////        QLineEdit * costForm1 = ui->costForm_lineEdit;
////        QLineEdit * costUse1 = ui->costUse_lineEdit;
//        //combox再说
//        if(costDept1->text().isEmpty()||costContra1->text().isEmpty()||costContraNum1->text().isEmpty()
//                ||costForm1->text().isEmpty()||costUse1->text().isEmpty())
//        {
//            QMessageBox::warning(this, tr("警告"),tr("请输入数据"),QMessageBox::Yes);
//            currentIndex = 13;
//        }
//        else
//        {
//            qDebug()<<tr("cost不为空");
//            //设置表头数据
//            exphead.setHeadCosttype(costType1->currentText());
//            exphead.setHeadcontractid(costContraNum1->text().trimmed());
//            exphead.setHeadContractname(costContra1->text().trimmed());
//            exphead.setHeaddeptid(costDept1->text().trimmed());
//            exphead.setHeadrequisitionid(costForm1->text().trimmed());//借款单
//            exphead.setHeadpurpose(costUse1->text().trimmed());

//            cost.setCostType_comboBox(costType1->currentText());
//            cost.setCostContra(costContra1->text().trimmed());
//            cost.setCostContraNum(costContraNum1->text().trimmed());
//            cost.setCostDept(costDept1->text().trimmed());
//            cost.setCostForm(costForm1->text().trimmed());
//            cost.setCostUse(costUse1->text().trimmed());

//            cost.toString();
//            expenseTypeId = 13;
//            if (busiType == true)//这里需要做判断 根据录入是否有车票报销,在确定是否直接跳转到支付页面
//            {
//                currentIndex = 12;
//            }
//            else
//            {
//                currentIndex = 15;
//            }

//        }
//        //确认界面显示费用报销单信息
//        confirmTable = ui->confirmExcep_Table;
//        confirmTable->setColumnCount(9);//设置列数
//        confirmTable->setRowCount(costTable->rowCount());
//        QStringList headers;
//        headers<<"报销单编号"<<"单据日期"<<"报销人"<<"报销金额"<<"单据张数"<<"预算项目"<<"预算归属部门"<<"借款单"<<"备注";
//        confirmTable->setHorizontalHeaderLabels(headers);
//        confirmTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//        for ( int i=0;i<costTable->rowCount();i++)
//        {
//            for (int j=0;j<costTable->columnCount();j++)
//            {
//                if(costTable->item(i,j)!=NULL)
//                {
//                    //                    bsTable=busTable->item(i,j)->text();
//                    confirmTable->setItem(i,j,new QTableWidgetItem(costTable->item(i,j)->text()));
//                }
//            }
//        }

        //    confirmTable->setItem (0,0,QTableWidgetItem(billNumber));
    }else if (currentIndex == 14)
    {
        //获取输入值，封装成busi对象
        QComboBox * abordType1 = ui->abordType_comboBox;
        QLineEdit * abordDept1 = ui->abordDept_lineEdit;
        QLineEdit * abordContra1 = ui->abordContra_lineEdit;
        QLineEdit * abordContraNum1 = ui->abordContraNum_lineEdit;
        QLineEdit * abordForm1 = ui->abordForm_lineEdit;
        QLineEdit * abordCause1 = ui->abordCause_lineEdit;
        //combox再说
        if(abordDept1->text().isEmpty()||abordContra1->text().isEmpty()||abordContraNum1->text().isEmpty()
                ||abordForm1->text().isEmpty()||abordCause1->text().isEmpty())
        {
            QMessageBox::warning(this, tr("警告"),tr("请输入数据"),QMessageBox::Yes);
            currentIndex = 14;
        }
        else
        {
            qDebug()<<tr("cost不为空");
            //设置报销单表头数据
            exphead.setHeadCosttype(abordType1->currentText());
            exphead.setHeadcontractid(abordContraNum1->text().trimmed());
            exphead.setHeadContractname(abordContra1->text().trimmed());
            exphead.setHeaddeptid(abordDept1->text().trimmed());
            exphead.setHeadrequisitionid(abordForm1->text().trimmed());//借款单
            exphead.setHeadpurpose(abordCause1->text().trimmed());

            abroad.setAbordType(abordType1->currentText());
            abroad.setAbordDept(abordDept1->text().trimmed());
            abroad.setAbordContra(abordContra1->text().trimmed());
            abroad.setAbordContraNum(abordContraNum1->text().trimmed());
            abroad.setAbordForm(abordForm1->text().trimmed());
            abroad.setAbordCause(abordCause1->text().trimmed());

            abroad.toString();
            expenseTypeId = 14;
            currentIndex = 15; //这里需要做判断 根据录入发票类型的种类,在确定是否直接跳转到支付页面
        }
        //确认界面显示出国报销单信息
        confirmTable = ui->confirmExcep_Table;
        confirmTable->setColumnCount(24);//设置列数
        confirmTable->setRowCount(abdTable->rowCount());
        QStringList headers;
        headers<<"报销单号"<<"出差人"<<"起始时间"<<"终止时间"<<"出差天数"<<"出发地点"<<"到达地点"<<"市间交通费"<<"交通费单据张数"
              <<"住宿费用"<<"住宿单据张数"<<"补贴天数"<<"交通补贴"<<"伙食补贴"<<"签证费金额"<<"签证费单据张数"<<"服务费金额"
             <<"服务费票据张数"<<"出国换汇金额"<<"出国换汇金额票据张数"<<"其他费用金额"<<"其他费用单据张数"<<"其他费用说明"<<"小计";
        confirmTable->setHorizontalHeaderLabels(headers);
        confirmTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        for ( int i=0;i<abdTable->rowCount();i++)
        {
            for (int j=0;j<abdTable->columnCount();j++)
            {
                if(abdTable->item(i,j)!=NULL)
                {
                    //                    bsTable=busTable->item(i,j)->text();
                    confirmTable->setItem(i,j,new QTableWidgetItem(abdTable->item(i,j)->text()));
                }
            }
        }
    }else if (currentIndex == 15)
    {

        //    QString payinfoType;//支付方式 待定
        //    QString payInfoUnit;//收款人
        //    QString payInfoBank;//所属银行
        //    QString payInfoBankName;//开户行名称
        //    QString payInfoBankNum;//银行卡号
        //    QString payInfoBill;//金额
        //    QString payInfoNote;//备注
        //获取输入值，封装成busi对象
        QComboBox * payinfoType1 = ui->payinfoType_comboBox;
        QLineEdit * payInfoUnit1 = ui->payInfoUnit_lineEdit;
        QLineEdit * payInfoBank1 = ui->payInfoBank_lineEdit;
        QLineEdit * payInfoBankName1 = ui->payInfoBankName_lineEdit;
        QLineEdit * payInfoBankNum1 = ui->payInfoBankNum_lineEdit;
        QLineEdit * payInfoBill1 = ui->payInfoBill_lineEdit;
        QLineEdit * payInfoNote1 = ui->payInfoNote_lineEdit;
        //combox再说
        if(payInfoUnit1->text().isEmpty()||payInfoBank1->text().isEmpty()
                ||payInfoBankName1->text().isEmpty()||payInfoBankNum1->text().isEmpty()/*||payInfoNote1->text().isEmpty()*/
                ||payInfoBill1->text().isEmpty())
        {
            QMessageBox::warning(this, tr("警告"),tr("请输入数据"),QMessageBox::Yes);
            currentIndex = 15;
        }
        else
        {
            qDebug()<<tr("payinfo不为空")<<endl;
            qDebug()<<tr("payinfoType1:")<<payinfoType1->currentText()<<endl;
            pay.setPayinfoType(payinfoType1->currentText());
            pay.setPayInfoUnit(payInfoUnit1->text().trimmed());
            pay.setPayInfoBank(payInfoBank1->text().trimmed());
            pay.setPayInfoBankName(payInfoBankName1->text().trimmed());
            pay.setPayInfoBankNum(payInfoBankNum1->text().trimmed());
            pay.setPayInfoBill(payInfoBill1->text().trimmed());
            pay.setPayInfoNote(payInfoNote1->text().trimmed());

            currentIndex = 16;
        }
    }
    else if(currentIndex < 17 && currentIndex > 0){
        currentIndex ++;
    }
    ui->mainWidget->setCurrentIndex(currentIndex);
    if(currentIndex == 16){

        switch(expenseTypeId){

        case 12:
            ui->confirmDept_lineEdit->setText(busi.getBusiDept_line());
            ui->confirmCause_lineEdit->setText(busi.getBusiCause_line());
            ui->confirmForm_lineEdit->setText(busi.getBusiForm_line());
            ui->confirmProject_lineEdit->setText(busi.getBusiProject_line());
            break;
        case 13:
            ui->confirmDept_lineEdit->setText(cost.getCostDept());
            ui->confirmCause_lineEdit->setText(cost.getCostUse());
            ui->confirmForm_lineEdit->setText(cost.getCostForm());
            break;
        case 14:
            ui->confirmDept_lineEdit->setText(abroad.getAbordDept());
            ui->confirmCause_lineEdit->setText(abroad.getAbordCause());
            ui->confirmForm_lineEdit->setText(abroad.getAbordForm());
            break;
        default:

            break;
        }

        //显示支付信息
        ui->ConfirmPayInfo_plainTextEdit->setText(pay.toString());
        //显示票据信息
        ui->ConfirmBillInfo_plainTextEdit->setText(bill.toString());
        player->stop();
        this->sendPlayText(" 请确认报销数据");
    }
}


void MainWindow::toCurrentPage (int pageNum)
{
    ui->mainWidget->setCurrentIndex (pageNum);
    switch (pageNum) {
    case 0:
        break;
    case 1:
        ui->firstBtn->show ();
        ui->lastStepBtn->show();

        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        //the next step we should init a carma
        //设置取景器
        viewfinder = new QCameraViewfinder(this);
        ui->faceRegImageLayout->addWidget(viewfinder);
        camera->setViewfinder(viewfinder);
        viewfinder->resize(600,400);

        imageCaptureReg = new QCameraImageCapture(camera);
        camera->setCaptureMode(QCamera::CaptureStillImage);
        imageCaptureReg->setCaptureDestination(QCameraImageCapture::CaptureToFile);

        //        camera->searchAndLock();
        camera->start();
        //        camera->unlock();

        //跳到人脸注册页面
        currentIndex = 7;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    case 7:
        //the next step ,wes should check the idcard
        if(m_currentRunLoaclThread)
        {
            m_currentRunLoaclThread->stopImmediately();
        }
        this->idCardRead();
        currentIndex=8;
        ui->mainWidget->setCurrentIndex (currentIndex);

        //clear idcard input box
        ui->idName_2->setText("");
        ui->idBirth_2->setText("");
        ui->idCardNum_2->setText("");
        ui->idHomeAddre_2->setText("");
        ui->idSex_2->setText("");
        ui->idNation_2->setText("");
        ui->idProvePlace_2->setText("");
        ui->idStartDay_2->setText("");
        ui->idEndDay_2->setText("");
        ui->idNewestAddre_2->setText("");
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 12:
        break;
    case 13:
        break;
    case 14:
        break;
    case 15:
        break;
    case 16:
        break;
    case 17:
        break;
    default:
        break;
    }
}


void MainWindow::on_lastStepBtn_clicked()
{
    player->stop();
    this->sendPlayText("上一步");

    qDebug() <<"当前页号"<<currentIndex;
    switch (currentIndex) {
    case 3:
        this->deleteFace ();
        break;
    case 5:
        this->idCardThreadStop ();
        break;
    case 4:
        this->deleteAccountInfo ();
        break;
    default:
        break;
    }
    this->toLastStep();
}

void MainWindow::on_nextStepBtn_clicked()
{
    player->stop();
    this->sendPlayText("下一步");
    this->toNextStep();
}
/*
 * currentIndex：
   0:welcomepage  //三个模块选择
   1:startpage  用户设置  登录还是注册// 显示上一步
   2:loginpage  登录方式 // 显示上一步
   3:face 人脸登录  //
   4:acount 账户登录  //上一步
   5:idCord 身份证登录  //上一步
   6:RegChoose 身份证认证 //上一步
   7:faceReg 人脸注册 //上一步
   8:accountReg:账号注册 //上一步
   9:begin:即将开始报销页 //上一步 下一步
   10:bill 票据录入 //上下
   11:expenseChoice 报销类别 //上一步
   12:busiExpense  差旅报销单 //上下
   13:costExpense 费用报销单 //上下
   14:abroadExpense 出国报销单 //上下
   15:payInfo  支付信息 // 上下
   16:confirm 信息确认 // 上一步
   17:finish 报销完成
*/
//当前页发生变化调用函数
void MainWindow::on_mainWidget_currentChanged(int arg1)
{
    ui->lastStepBtn->hide();
    ui->nextStepBtn->hide();
    //0、1/2/3/4
    //页号为2、、5、6、7、9、10的时候显示上一步
    if(currentIndex >1 && currentIndex <16){
        ui->lastStepBtn->show();
    }
    //页号为4、5、6、7、8、9的时候显示下一步
    //    if(currentIndex>9 && currentIndex <15){
    //        ui->nextStepBtn->show();
    //    }
    if(currentIndex == 8 || currentIndex == 9 || currentIndex == 10 /*||currentIndex == 12 ||currentIndex == 13 || currentIndex == 14*/ ||currentIndex==15){
        ui->nextStepBtn->show();
    }
    //    if(currentIndex == 8 )
    //    {
    //        this->toNextStep();
    //        //页号为8的时候启动定时器，2s后开始报销
    //        //        timerWelcom->setInterval(2000);
    //        //        timerWelcom->start();
    //        //        connect(timerWelcom, SIGNAL(timeout()), this, SLOT(beginWork()));
    //    }
    //页号为10,的时候可处理页面表格
    if(currentIndex == 12 ){// && busiTalble == false
        //        QTableWidget * table = ui->busiBodyTable;
        ui->nextStepBtn->show();

//        bsTable = ui->busiBodyTable;
//        busiTalble = true;
//        bsTable->setColumnCount(18);//设置列数
//        bsTable->setRowCount(1);//设置行数
//        /*设置列名*/
//        QStringList headers;
//        headers<<"报销单号"<<"出差人"<<"起始时间"<<"终止时间"<<"出差天数"<<"出发地点"<<"到达地点"<<"市间交通费"<<"交通费单据张数"<<"住宿费用"<<"住宿单据张数"<<"补贴天数"<<"交通补贴"<<"伙食补贴"<<"其他费用金额"<<"其他费用单据张数"<<"其他费用说明"<<"小计";
//        bsTable->setHorizontalHeaderLabels(headers);
//        bsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//        bsTable->setItem (0,1,new QTableWidgetItem(trainPerson));
//        bsTable->setItem (0,2,new QTableWidgetItem(trainDate));
//        bsTable->setItem (0,5,new QTableWidgetItem(trainStart));
//        bsTable->setItem (0,6,new QTableWidgetItem(trainEnd));
         // ui->nextStepBtn->show();

    }
    if(currentIndex == 13 ){
       ui->nextStepBtn->show();

//        //        QTableWidget * table = ui->busiBodyTable;
//        costTable = ui->costBodyTable;
//        costTable->setColumnCount(9);//设置列数
//        costTable->setRowCount(1);//设置行数
//        /*设置列名*/
//        QStringList headers;
//        headers<<"报销单编号号"<<"单据日期"<<"币种"<<"电子发票号"<<"纳税人识别号"<<"报销人"<<"报销金额"<<"单据张数"<<"预算项目";
//        costTable->setHorizontalHeaderLabels(headers);
//        costTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//        costTable->setItem (0,0,new QTableWidgetItem(billNumber));
//        costTable->setItem (0,1,new QTableWidgetItem(fbillDate));
//        costTable->setItem (0,2,new QTableWidgetItem("人民币"));
//        costTable->setItem (0,3,new QTableWidgetItem(billCode));
//        costTable->setItem (0,4,new QTableWidgetItem(billCheckcode));
//        costTable->setItem (0,6,new QTableWidgetItem(billPrice));
//        costTable->setItem (0,7,new QTableWidgetItem("1"));
      //ui->nextStepBtn->show();

  }
    if(currentIndex == 14){
        ui->nextStepBtn->show();

//        abdTable = ui->abdBodyTable;
//        abdTable->setColumnCount(24);//设置列数
//        abdTable->setRowCount(1);//设置行数
//        /*设置列名*/
//        QStringList headers;
//        headers<<"报销单号"<<"出差人"<<"起始时间"<<"终止时间"<<"出差天数"<<"出发地点"<<"到达地点"<<"市间交通费"<<"交通费单据张数"<<"住宿费用"<<"住宿单据张数"<<"补贴天数"<<"交通补贴"<<"伙食补贴"<<"签证费金额"<<"签证费单据张数"<<"服务费金额"<<"服务费票据张数"<<"出国换汇金额"<<"出国换汇金额票据张数"<<"其他费用金额"<<"其他费用单据张数"<<"其他费用说明"<<"小计";
//        abdTable->setHorizontalHeaderLabels(headers);
//        abdTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
}
void MainWindow::dealBusi ()
{
    bsTable = ui->busiBodyTable;
    //busiTalble = true;
    bsTable->setColumnCount(7);//设置列数
    busiRowcount = bsTable->rowCount ();
    bsTable->setRowCount(busiRowcount+1);//设置行数
    /*设置列名*/
    QStringList headers;
    headers<<"报销单号"<<"出差人"<<"起始时间"<<"终止时间"<<"出差天数"<<"出发地点"<<"到达地点";
    bsTable->setHorizontalHeaderLabels(headers);
    bsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    bsTable->setItem (busiRowcount,1,new QTableWidgetItem(trainPerson));
    bsTable->setItem (busiRowcount,2,new QTableWidgetItem(trainDate));
    bsTable->setItem (busiRowcount,5,new QTableWidgetItem(trainStart));
    bsTable->setItem (busiRowcount,6,new QTableWidgetItem(trainEnd));
}

//void MainWindow::dealCost ()
//{
//    //        QTableWidget * table = ui->busiBodyTable;
//    costTable = ui->costBodyTable;
//    costTable->setColumnCount(9);//设置列数
//    costRowcount = costTable->rowCount ();
//    costTable->setRowCount(costRowcount+1);//设置行数
//    /*设置列名*/
//    QStringList headers;
//    headers<<"报销单编号号"<<"单据日期"<<"币种"<<"电子发票号"<<"纳税人识别号"<<"报销人"<<"报销金额"<<"单据张数"<<"预算项目";
//    costTable->setHorizontalHeaderLabels(headers);
//    costTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    costTable->setItem (costRowcount,0,new QTableWidgetItem(billNumber));
//    costTable->setItem (costRowcount,1,new QTableWidgetItem(fbillDate));
//    costTable->setItem (costRowcount,2,new QTableWidgetItem("人民币"));
//    costTable->setItem (costRowcount,3,new QTableWidgetItem(billCode));
//    costTable->setItem (costRowcount,4,new QTableWidgetItem(billCheckcode));
//    costTable->setItem (costRowcount,6,new QTableWidgetItem(billPrice));
//    costTable->setItem (costRowcount,7,new QTableWidgetItem("1"));
//}
void MainWindow::dealAbroad ()
{
    abdTable = ui->abdBodyTable;
    abdTable->setColumnCount(24);//设置列数
    abroadRowcount =abdTable->rowCount ();
    abdTable->setRowCount(abroadRowcount+1);//设置行数
    /*设置列名*/
    QStringList headers;
    headers<<"报销单号"<<"出差人"<<"起始时间"<<"终止时间"<<"出差天数"<<"出发地点"<<"到达地点"<<"市间交通费"<<"交通费单据张数"<<"住宿费用"<<"住宿单据张数"<<"补贴天数"<<"交通补贴"<<"伙食补贴"<<"签证费金额"<<"签证费单据张数"<<"服务费金额"<<"服务费票据张数"<<"出国换汇金额"<<"出国换汇金额票据张数"<<"其他费用金额"<<"其他费用单据张数"<<"其他费用说明"<<"小计";
    abdTable->setHorizontalHeaderLabels(headers);
    abdTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

//选择差旅报销单
void MainWindow::on_busiBtn_clicked()
{
    player->stop();
    this->sendPlayText("已选择差旅报销");

    currentIndex = 13;
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->nextStepBtn->show ();




    //    this->toNextStep();
}

//选择费用报销单
void MainWindow::on_costBtn_clicked()
{
    player->stop();
    this->sendPlayText("已选择费用报销");
    currentIndex = 13;
    expenseType = 13;
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->nextStepBtn->show ();
    //返回的数据添加至table中
    this->isShowguiInforn ();




}

//选择出国保险单
void MainWindow::on_abroadBtn_clicked()
{
    player->stop();
    this->sendPlayText("已选择出国报销");

    currentIndex = 11;
    expenseType = 14;
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->nextStepBtn->show ();



}

//增行
void MainWindow::on_busiBodyAddBtn_clicked()
{
    int rowCount = ui->busiBodyTable->rowCount();
    rowCount ++ ;
    ui->busiBodyTable->setRowCount(rowCount);
}

//删行
void MainWindow::on_busiBodyDelBtn_clicked()
{
    int rowIndex = ui->busiBodyTable->currentRow();
    if(rowIndex != -1){
        ui->busiBodyTable->removeRow(rowIndex);
    }
}

//继续报销
void MainWindow::on_continueExpenseBtn_clicked()
{
    player->stop();
    this->sendPlayText("继续报销");
    costRowcount= 1; //费用报销单rowcount为1
    //clear table
    // busi.init ();
    currentIndex = 9;
    expenseType = 9;
//    switch (reimCategory) {  //修改报销流程之后需要针对 资源回收做对应的修改.
//    case 0: //差旅报销
//        this->deleteBillInput ();
//        this->deleteTravelContent ();
//        this->deletePayInforn ();
//        this->deleteInfornConfirm ();
//        break;
//    case 1: //费用报销
//        this->deleteBillInput ();
//        this->deleteCostContent ();
//        this->deletePayInforn ();
//        this->deleteInfornConfirm ();
//        break;
//    case 2: //出国报销
//        this->deleteBillInput ();
//        this->deleteAbroadContent ();
//        this->deletePayInforn ();
//        this->deleteInfornConfirm ();
//        break;
//    default:
//                   qDebug () << "测试";
//        break;
//    }
    if (costType ==true)
    {
        this->deleteCostContent ();
    }
    if (busiType ==true)
    {
        this->deleteTravelContent ();

    }
    if (abroadType == true)
    {
        this->deleteAbroadContent ();
    }
    this->deleteBillInput ();
    this->deletePayInforn ();
    this->deleteInfornConfirm ();
    ui->mainWidget->setCurrentIndex(currentIndex);


}
/**
* @brief         清空登录信息
* @author        胡坤伦
* @date        2019-03-11
*/
void MainWindow::deleteAccountInfo ()
{
    ui->user_loginEdit->clear ();
    ui->user_pwdEdit->clear ();
}
/**
* @brief         清空注册信息
* @author        胡坤伦
* @date        2019-03-11
*/
void MainWindow::deleteAccountReg ()
{
    ui->RegPwd_LineEdit->clear ();
    ui->RegPhoneNum_LineEdit->clear ();
    ui->RegUsername_LineEdit->clear ();
}

/**
* @brief         回收人脸识别资源
* @author        胡坤伦
* @date        2019-03-11
*/
void MainWindow::deleteFace ()
{
    qDebug() << "销毁人脸登录";
    camera->stop();
    if(timer->isActive ())
    {
        timer->stop ();
    }
}

/**
* @brief         清空录入票据
* @author        胡坤伦
* @date        2019-03-08
*/
void MainWindow::deleteBillInput ()
{
    int rowIndex = table->rowCount ();
    qDebug() << "行数:"<<rowIndex;
    for (int i =0 ;i<rowIndex;i++)
    {
        qDebug() << "删除行数:"<<i;
        table->removeRow (0);
    }

    //ui->billTable->clearContents ();
}
/**
* @brief         清空费用报销内容
* @author        胡坤伦
* @date        2019-03-08
*/
void MainWindow::deleteCostContent ()
{

    int rowIndex = costTable->rowCount ();
    qDebug() << "行数:"<<rowIndex;
    for (int i =0 ;i<rowIndex;i++)
    {
        qDebug() << "删除行数:"<<i;
        costTable->removeRow (0);
    }
//    ui->costDept_lineEdit->clear ();
//    ui->costUse_lineEdit->clear ();
//    ui->costContra_lineEdit->clear();
//    ui->costContraNum_lineEdit->clear ();
//    ui->costForm_lineEdit->clear ();
    costType = false;
}
/**
* @brief         清空差旅报销
* @author        胡坤伦
* @date        2019-03-08
*/
void MainWindow::deleteTravelContent ()
{
    //bsTable->clearContents ();
    int rowIndex = bsTable->rowCount ();
    qDebug() << "行数:"<<rowIndex;
    for (int i =0 ;i<rowIndex;i++)
    {
        qDebug() << "删除行数:"<<i;
        bsTable->removeRow (0);
    }
    ui->busiCause_lineEdit->clear ();
    ui->busiDept_lineEdit->clear ();
    ui->busiForm_lineEdit->clear ();
    ui->busiProject_lineEdit->clear ();
    busiType = false;
}
/**
* @brief         清空出国报销
* @author        胡坤伦
* @date        2019-03-08
*/
void MainWindow::deleteAbroadContent ()
{
    //abdTable->clearContents ();
    int rowIndex = abdTable->rowCount ();
    qDebug() << "行数:"<<rowIndex;
    for (int i =0 ;i<rowIndex;i++)
    {
        qDebug() << "删除行数:"<<i;
        abdTable->removeRow (0);
    }
    ui->abordCause_lineEdit->clear ();
    ui->abordDept_lineEdit->clear ();
    ui->abordContra_lineEdit->clear ();
    ui->abordContraNum_lineEdit->clear ();
    ui->abordForm_lineEdit->clear ();
    abroadType = false;
}
/**
* @brief         清空支付信息
* @author        胡坤伦
* @date        2019-03-08
*/
void MainWindow::deletePayInforn ()
{
    ui->payInfoUnit_lineEdit->clear ();
    ui->payInfoBank_lineEdit->clear ();
    ui->payInfoBankName_lineEdit->clear ();
    ui->payInfoBankNum_lineEdit->clear ();
    ui->payInfoBill_lineEdit->clear();
    ui->payInfoNote_lineEdit->clear ();
}
/**
* @brief         清空确认信息
* @author        胡坤伦
* @date        2019-03-08
*/
void MainWindow::deleteInfornConfirm ()
{
    ui->ConfirmBillInfo_plainTextEdit->clear ();
    ui->ConfirmPayInfo_plainTextEdit->clear ();
    ui->confirmExcep_Table->clearContents ();
    ui->confirmCause_lineEdit->clear();
    ui->confirmDept_lineEdit->clear ();
    ui->confirmForm_lineEdit->clear ();
    ui->confirmProject_lineEdit->clear ();
}


//费用报销单增行
//void MainWindow::on_costBodyAddBtn_clicked()
//{
//    //int rowCount = ui->costBodyTable->rowCount();
//    costRowcount = ui->costBodyTable->rowCount ();
//    costRowcount++;
//    ui->costBodyTable->setRowCount(costRowcount);
//}
//费用报销单减行
//void MainWindow::on_costBodyDelBtn_clicked()
//{
//     costRowcount = ui->costBodyTable->currentRow();
//    if( costRowcount != -1){
//        ui->costBodyTable->removeRow( costRowcount);
//    }
//}
//出国报销单增行
void MainWindow::on_pushButton_clicked()
{
    int rowCount = ui->abdBodyTable->rowCount();
    rowCount ++ ;
    ui->abdBodyTable->setRowCount(rowCount);

}
//出国报销单减行
void MainWindow::on_pushButton_2_clicked()
{
    int rowIndex = ui->abdBodyTable->currentRow();
    if(rowIndex != -1){
        ui->abdBodyTable->removeRow(rowIndex);
    }
}

//结束报销
void MainWindow::on_logoutBtn_clicked()
{
    player->stop();
    this->sendPlayText("退出");
    currentIndex =0;
    expenseType = 9;
    ui->userInfo->hide();
    //    delete loginUser;
    //    delete loginInformation;
    //    delete idCardInformation;
    //    ui->userInfoCheck->setText("");
    if (costType ==true)
    {
        this->deleteCostContent ();
    }
    if (busiType ==true)
    {
        this->deleteTravelContent ();

    }
    if (abroadType == true)
    {
        this->deleteAbroadContent ();
    }
    this->deleteBillInput ();
    this->deletePayInforn ();
    this->deleteInfornConfirm ();
    this->deleteAccountInfo ();
    ui->mainWidget->setCurrentIndex(currentIndex);
}

//-------------------------------------------sequence action end----------------------------------------



//-------------------------------------------slots start----------------------------------------
/**
* @function   photoRegister
* @brief      人脸注册结果处理
* @author        胡想成
* @date          2018-08-17
*/
void MainWindow::photoRegister(int id,QImage image)
{
    if(idFace==1){
        QByteArray fdata = this->getPixmapData("/files/regFace/",image);
        camera->stop();//停止相机拍照

        //准备发送网络请求和接收应答
        manager = new QNetworkAccessManager(this);
        QUrlQuery params;
        //不能直接到第二步
        if(regCode == 1)
        {
            QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceRegReply(QNetworkReply*)));

            fdata = fdata.toBase64();
            fdata = fdata.replace("+","-");
            fdata = fdata.replace("/","_");
            params.addQueryItem("image",fdata);
            params.addQueryItem("group_id","1");
            params.addQueryItem("uid",regUser.getUid());
            params.addQueryItem("user_info",regUser.getUsername());
            params.addQueryItem("imageType",".jpg");

            QString  data = params.toString();

            //打印请求参数(测试)
            this->printFile("reply.txt", fdata);
            QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/register/"));
            request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
            manager->post(request,params.toString().toUtf8());
        }else//人脸已存在
        {
            QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("请先注册账号！"));
        }
    }
}

/**
* @brief       人脸注册结果处理
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::faceRegReply(QNetworkReply *reply){

    //拿到返回结果
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();

        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
            if(doucment.isObject()){  // JSON 文档为对象
                QJsonObject object = doucment.object();  //  转化为对象
                qDebug()<< "----faceReg result:----"<<QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","") <<endl;
                /*
                    注册成功
                    {
                        "log_id": 2764578038040222
                    }
                    注册失败

                    {
                        "error_code": 216616,
                        "log_id": 674786177,
                        "error_msg": "image exist"
                    }
                 */
                QString error_msg="";

                //注册失败
                if(object.contains("error_code")){
                    QJsonValue numValue = object.value("error_code");
                    //                    QJsonValue numValue2 = object.value("error_msg");
                    if(numValue.isDouble()){
                        //                        error_code = numValue.toInt();
                    }
                    qDebug()<< "----result failure:----"<< error_msg<<endl;
                    QMessageBox::information(this, QString::fromUtf8("警告!"),QString::fromUtf8("人脸注册失败!"));

                    //                    currentIndex = 4;
                    //                    ui->mainWidget->setCurrentIndex(currentIndex);
                    //                    this->on_facePreBtn_clicked();
                    toCurrentPage (6);
                }
                //注册成功
                else{
                    //取log_id  16位字符串
                    log_id =  QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","").mid(8,16);
                    qDebug() <<"log_id:"<<log_id<<endl;
                    qDebug() <<"user:"<< regUser.toString()<<endl;

                    //注册成功插入数据库
                    if(database.CheckUserByUid(regUser.getUid()))
                    {
                        player->stop();
                        this->sendPlayText("人脸注册成功");
                        QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("人脸注册成功！"));
                        //认证身份证
                        idCheckId = 1;//注册的时候调用
                        regCode = 2;//人脸注册完成
                        toCurrentPage (7);
                    }
                    else
                    {
                        player->stop();
                        this->sendPlayText("账户注册失败");
                        regCode = 0;
                        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("注册失败"));

                        //                        currentIndex = 2;
                        //                        ui->mainWidget->setCurrentIndex(currentIndex);
                        //                        this->on_facePreBtn_clicked();
                        toCurrentPage (6);
                    }
                }
                //清空内容并定位光标
                ui->RegUsername_LineEdit->clear();
                ui->RegPwd_LineEdit->clear();
                ui->RegPhoneNum_LineEdit->clear();
                ui->avaterLineEdit->clear();
                ui->avaterRegBoxLabel->clear();
            }
        }
    }
}


/**
* @function   faceCheck
* @brief        检测人脸库登录
* @author        胡想成
* @date          2018-08-17
*/
void MainWindow::faceCheck(){
    if(QCamera::ActiveStatus == camera->status())
    {
        qDebug()<<"第" << facetime << "次查询人脸";
        facetime++;
        idFace = 2;//登录查人脸

        //take photos
        imageCapture->capture();
        if(facetime < 5)
        {
            connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(sendPhoto(int,QImage)));
        qDebug() << "face check success,111111111"<< endl;

        }else
        {
            qDebug() << "face check failed, stop send photos"<< endl;
            //            QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("人脸不存在，请重新注册"));
            if(timer->isActive ())
            {
                timer->stop ();
            }
            camera->stop ();
            currentIndex =2;
            ui->mainWidget->setCurrentIndex(currentIndex);
        }

    }
}
/**
* @function   faceReply
* @brief       人脸登录结果处理
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::faceReply(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError) && doucment.isObject()) {

            QJsonObject object = doucment.object();
            //            qDebug()<< QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","")<<endl;
            //获取result_num参数
            int result_num = 0;
            if(object.contains("result_num"))
            {
                QJsonValue numValue = object.value("result_num");
                if(numValue.isDouble()){
                    result_num = numValue.toInt();
                }
            }
            if(result_num >=1 && object.contains("result"))
            {//人脸识别有结果
                QJsonValue resultsValue = object.value("result");
                if(resultsValue.isArray())
                {
                    QJsonArray resultsArray = resultsValue.toArray();
                    qDebug() << "resultsArray:" << resultsArray;

                    QString uid;
                    QString user_info;
                    double score = 80;//阈值为80分
                    //                    int nSize = resultsArray.size();
                    for(int i = 0  ; i <  resultsArray.size(); i ++){
                        QJsonValue resultValue = resultsArray.at(i);
                        double scoreTemp = 0;
                        if(resultValue.isObject()){
                            QJsonObject resultObj = resultValue.toObject();
                            //获取相似度分数
                            if(resultObj.contains("scores")){
                                QJsonValue scoresVal = resultObj.value("scores");
                                if(scoresVal.isArray()){
                                    QJsonArray scoresArray = scoresVal.toArray();
                                    QJsonValue scoreVal = scoresArray.at(0);
                                    if(scoreVal.isDouble()){
                                        scoreTemp = scoreVal.toDouble();
                                        qDebug() << "scoreTemp:" << scoreTemp;
                                    }
                                }
                            }
                            if(scoreTemp > score){
                                score = scoreTemp;
                                //获取唯一uid
                                if(resultObj.contains("uid")){
                                    QJsonValue uidVal = resultObj.value("uid");
                                    if(uidVal.isString()){
                                        uid = uidVal.toString();
                                    }
                                }
                                // 获取user_info
                                if(resultObj.contains("user_info")){
                                    QJsonValue infoVal = resultObj.value("user_info");
                                    if(infoVal.isString()){
                                        user_info = infoVal.toString();
                                    }
                                }
                            }
                        }
                    }
                    if(score > 80){
                        //相似度分数大于80,识别成功
                        qDebug() << "user_info:" << user_info;
                        qDebug() << "uid:" << uid;
                        uname = user_info;

                        if(!database.CheckUserByUid(uid))
                        {
                            //数据库不存在此人脸信息
                            //                            player->stop();
                            //                            this->sendPlayText("人脸不存在，请重新注册！");
                            //请求网络 删除人脸库数据
                            this->deletePhoto(uid);
                            //                            QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("人脸不存在，请重新注册"));
                            qDebug() << "人脸不存在，请重新注册" << uid;

                        }else
                        {
                            isFaceOk = true;
                            loginStatus = 1;//人脸已登录
                            //成功登陆
                            loginUser = database.SearchUserByUid(uid);
                            qDebug() << "loginUser:" << loginUser.toString();

                            this->showUserInfo(loginUser);
                            player->stop();
                            this->sendPlayText("登录成功");

                            camera->stop();
                            if(timer->isActive ())
                            {
                                timer->stop ();
                            }
                            currentIndex = 9;
                            ui->lastStepBtn->show ();
                            ui->nextStepBtn->show ();
                            ui->mainWidget->setCurrentIndex(currentIndex);
                        }
                    }
                }
            }

        }
    }

}


/**
* @brief       人脸删除处理
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::faceDelReply(QNetworkReply *reply){

    qDebug()<<"----删除photo:----"<<reply->error()<<endl;
    //拿到返回结果
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        //打印结果
        qDebug()<<"----处理反斜杠:----"<< all <<endl;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
            if(doucment.isObject()){  // JSON 文档为对象
                QJsonObject object = doucment.object();  //  转化为对象

                qDebug()<< "----faceDelete result:----"<<QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","") <<endl;
                /*
                    删除成功
                    {
                        "log_id": 2764578038040222
                    }
                    删除失败

                    {
                        "error_code": 216616,
                        "log_id": 674786177,
                        "error_msg": "image exist"
                    }
                 */
                QString error_msg="";

                //删除失败
                if(object.contains("error_code")){
                    QJsonValue numValue = object.value("error_code");
                    QJsonValue numValue2 = object.value("error_msg");
                    if(numValue.isDouble()){
                        //                        error_code = numValue.toInt();
                    }

                    qDebug() <<"delete:"<< "人脸删除失败"<<endl;
                    qDebug()<< "----delete failure:----"<< error_msg <<endl;
                    //                    QMessageBox::information(this, QString::fromUtf8("警告!"),QString::fromUtf8("人脸注册失败!"));
                }
                //删除成功
                else{
                    //取log_id  16位字符串
                    log_id =  QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","").mid(8,16);
                    //                    qDebug() <<"log_id:"<<log_id<<endl;
                    qDebug() <<"delete:"<< "人脸删除成功"<<endl;
                }
            }
        }
    }
}



/**
* @brief       身份证识别返回结果处理
* @author        胡帅成
* @date        2018-09-07
*/
void MainWindow::dealIdCardReply(Information information,int exitCode)
{
    //    if(m_currentRunLoaclThread)
    //    {
    //        m_currentRunLoaclThread->stopImmediately();
    //    }

    qDebug()<<"exitCode:"<< exitCode << endl;


    if(exitCode == 0)
    {
        idCardInformation = information;
        qDebug()<<"idCardInformation:"<< idCardInformation.toString ()<< endl;

        switch (idCheckId) {
        case 0://未登录
            break;
        case 1:// 注册身份证
            ui->idName_2->setText(QString::fromStdString(information.getName()));
            ui->idBirth_2->setText(QString::fromStdString(information.getBirthday()));
            ui->idCardNum_2->setText(QString::fromStdString(information.getIdNumber()));
            ui->idHomeAddre_2->setText(QString::fromStdString(information.getAddress()));
            ui->idSex_2->setText(QString::fromStdString(information.getSex()));
            ui->idNation_2->setText(QString::fromStdString(information.getNation()));
            ui->idProvePlace_2->setText(QString::fromStdString(information.getSigningOrganization()));
            ui->idStartDay_2->setText(QString::fromStdString(information.getValidityPeriodStart()));
            ui->idEndDay_2->setText(QString::fromStdString(information.getValidityPeriodEnd()));
            ui->idNewestAddre_2->setText(QString::fromStdString(information.getNewestAddress()));

            player->stop();
            this->sendPlayText("身份证识别成功！");
            QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("识别成功"));
            break;
        case 2://身份证登录+
            if(!idCardInformation.getName().empty())
            {
                qDebug()<<"idCardInformation:不为空:"<< QString::fromStdString(idCardInformation.getIdNumber())<<endl;
                //判断information不为空
                if(database.CheckUserByIdCard(QString::fromStdString(idCardInformation.getIdNumber())))
                {
                    player->stop();
                    this->sendPlayText("身份证登录成功");

                    loginStatus = 2;//身份证登录状态
                    isIdcardChecked = true;
                    //用户表中有此用户,可以登录
                    loginUser  = database.SearchUserByIdNumber(QString::fromStdString(idCardInformation.getIdNumber()));
                    loginInformation = database.searchIdCardByIdNumber(QString::fromStdString(idCardInformation.getIdNumber()));//以本地数据库注册信息为准
                    //展示登录成功界面
                    ui->idUsername->setText("欢迎用户："+loginUser.getUsername());

                    ui->idLabel->show();
                    ui->idLabel2->show();
                    ui->idLabel3->show();
                    ui->idLabel4->show();
                    ui->idLabel5->show();

                    ui->idName->setText(QString::fromStdString(information.getName()));
                    ui->idAddress->setText(QString::fromStdString(information.getAddress()));
                    ui->idCardNumber->setText(QString::fromStdString(information.getIdNumber()));
                    ui->idSex->setText(QString::fromStdString(information.getSex()));
                    ui->idPhone->setText(loginUser.getPhoneNumber());
                    //隐藏数据显示框
                    ui->idName->show();
                    ui->idAddress->show();
                    ui->idCardNumber->show();
                    ui->idPhone->show();
                    ui->idSex->show();
                }
                else
                {
                    player->stop();
                    this->sendPlayText("此身份信息未认证！");

                    loginStatus = 0;
                    QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("身份信息未注册！"));
                    currentIndex =2;
                    ui->mainWidget->setCurrentIndex(currentIndex);
                }
            }
            break;
        default:
            break;
        }


    }
    else
    {

        player->stop();
        this->sendPlayText("身份证信息读取有误！");
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("身份证读取有误！"));
        //跳转到登录选择页
        //身份信息读取有误后回到原页面
        if(idCheckId==1)
        {
            toCurrentPage (7);}
        else
        {
//            on_idBtn_clicked();
            currentIndex =2;
            ui->mainWidget->setCurrentIndex (currentIndex);
        }
        //        ui->mainWidget->setCurrentIndex(2);
    }
}
/**
* @function   sendPhoto
* @brief         发送图片查找人脸库
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::sendPhoto(int id,QImage image){

    if(idFace == 2 && !isFaceOk)
    {
        QByteArray fdata = this->getPixmapData("/files/face",image);
        manager  = new QNetworkAccessManager(this);
        //发送结束后，进行人脸结果处理
        QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceReply(QNetworkReply*)));

        //封装请求参数(看接口文档)
        QUrlQuery params;
        fdata = fdata.toBase64().replace("+","-").replace("/","_");
        params.addQueryItem("image",fdata);
        params.addQueryItem("group_id","1");
        params.addQueryItem("imageType",".jpg");

        QString  data = params.toString();
        QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/search/"));
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
        manager->post(request,params.toString().toUtf8());
        qDebug() <<"11111111111111111";
    }
}

/**
* @function   sendPhoto
* @brief         删除人脸
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::deletePhoto(QString uid){
    qDebug()<<"deletePhoto:"<< uid;
    //准备发送网络请求和接收应答
    manager = new QNetworkAccessManager(this);
    //发送结束后，进行人脸结果处理
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceDelReply(QNetworkReply*)));

    //封装请求参数(看接口文档)
    QUrlQuery params;
    params.addQueryItem("uid", uid);
    params.addQueryItem("group_id","1");
    QString  data = params.toString();
    //    QString address = HOST.append("/face/delete/");

    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/search/"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->post(request,params.toString().toUtf8());
}



/**
* @brief         获取编码图片
* @author        胡帅成
* @date        2018-09-13
*/
QByteArray MainWindow::getPixmapData(QString filePath,QImage image)
{
    //    qDebug()<<"发送照片";
    QString baseDir = QCoreApplication::applicationDirPath();
    QString fileDir = baseDir.append(filePath);

    //创建目录
    QDir dir;
    if(!dir.exists(fileDir))
    {
        bool res = dir.mkpath(fileDir);
        qDebug() << "创建人脸图片目录结果:" << res;
    }

    fileDir.append(commonutils.getCurrentTime());
    fileDir.append(".jpg");

    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap.save(fileDir);
    qDebug()<<fileDir<<endl;

    QFile file(fileDir);

    if (!file.open(QIODevice::ReadOnly)||file.size()==0)
    {
        file.close();
        qDebug() << "文件打开失败";
    }
    QByteArray fdata = file.readAll();
    file.close();
    return fdata;
}


/**
* @brief       票据识别返回处理
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::billReply(QNetworkReply * reply){
    //打印结果
    qDebug() << "------票据识别结果------"<<reply->error()<< endl;
    if(reply->error() ==    QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        qDebug()<<endl<<endl<<all<<endl;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        if((!doucment.isEmpty()) && jsonError.error == QJsonParseError::NoError){//解析未发生错误
            if(doucment.isObject()){
                QJsonObject object = doucment.object();
                if(object.contains("success")){
                    int success = 0;
                    QJsonValue successVal = object.value("success");
                    if(successVal.isDouble()){
                        success = successVal.toInt();
                    }
                    if(success == 1){
                        //保存票据电子版附件
                        QString baseDir = QCoreApplication::applicationDirPath();
                        QString fileDir = baseDir.append("/files/bill/");
                        //创建目录
                        QDir dir;
                        if(!dir.exists(fileDir))
                        {
                            bool res = dir.mkpath(fileDir);
                            qDebug() << "创建保存票据电子版图片目录结果:" << res;
                        }
                        fileDir.append(commonutils.getCurrentTime());

                        //获得票据附件名称
                        QFileInfo filinfo;
                        filinfo = QFileInfo(fileDir);
                        billname = filinfo.fileName();
                        qDebug()<<"billname"<<billname;
                        fileDir.append(".jpg");
                        billdir = fileDir;
                        qDebug()<<"billdir"<<billdir;
                        blpath.append(billdir);
                        QImage img(billpath);
                        QPixmap pixmap = QPixmap::fromImage(img);
                        pixmap.save(fileDir);
                        bldir.append(billname);//保存票据识别的附件名称

                        int billType = 0;
                        if(object.contains("type")){
                            QJsonValue typeVal = object.value("type");
                            if(typeVal.isDouble()){
                                billType = typeVal.toInt();
                                bill.setBilltype(QString(billType));//票据类别
                            }
                        }
                        QJsonObject infoObj;
                        if(object.contains("info")){
                            QJsonValue infoVal = object.value("info");
                            if(infoVal.isObject()){
                                infoObj = infoVal.toObject();
                            }
                        }

                        /* {
                            "success": 1,
                            "type": 1,
                            "info": {
                                "code": "1100171320",
                                "number": "39283562",
                                "date": "2017年08月13日",
                                "goods": "餐费",
                                "price": "1888.00",
                                "checkCode": "04844754757667122571"
                            }
                        }
                        */

                        /*
                            QString billID;//报销人id
                            QString billExpenseid;
                            QString billAttachmenttitle;//附件名称
                            QString billRealpath;//文件保存路径
                            QString billType;//票据类型
                            QString billContent;//票据名目
                            QString billCode;//票据代码
                            QString billNumber;//票据号码
                            QString billUser;//火车票、飞机票乘车人
                            QString billStartplace;//始发站
                            QString billEndplace;//终点站
                            QString billDate;//发票日期、乘车日期
                            QString billMoney;//票据金额
                            QString billCheckcode;//校验码
                            QString billOther;//发票的号码*/

                        QString type;
                        QString billContent;
                        QString user;
                        QString start;
                        QString end;
                        QString billDate;
                        QString money;
                        QString other;
                        switch (billType) {
                        case 1://发票
                            type = "发票";
                            costType =true;
                            player->stop();
                            this->sendPlayText("发票识别成功");
                            bill.setBilltype(type);
                            if(infoObj.contains("goods")){
                                QJsonValue goodsVal = infoObj.value("goods");
                                if(goodsVal.isString()){
                                    billContent = goodsVal.toString();
                                    billGoods = billContent;
                                    bill.setBillcontent(billContent);
                                }
                            }
                            if(infoObj.contains("date")){
                                QJsonValue dateVal = infoObj.value("date");
                                if(dateVal.isString()){
                                    fbillDate = dateVal.toString();

                                    bill.setBilldate(fbillDate);
                                }
                            }
                            if(infoObj.contains("price")){
                                QJsonValue priceVal = infoObj.value("price");
                                if(priceVal.isString()){
                                    money = priceVal.toString();
                                    billPrice = money;
                                    bill.setBillmoney(money);
                                }
                            }
                            if(infoObj.contains("number")){
                                QJsonValue numberVal = infoObj.value("number");
                                if(numberVal.isString()){
                                    other = numberVal.toString();
                                    billNumber = other;
                                    bill.setBillnumber(other);
                                }
                            }
                            if(infoObj.contains("code")){
                                QJsonValue numberVal = infoObj.value("code");
                                if(numberVal.isString()){
                                    billCode = numberVal.toString();
                                    bill.setBillcode(numberVal.toString());
                                }
                            }
                            if(infoObj.contains("checkCode")){
                                QJsonValue numberVal = infoObj.value("checkCode");
                                if(numberVal.isString()){
                                    billCheckcode = numberVal.toString();
                                    bill.setBillcheckcode(numberVal.toString());
                                }

                            }
                            emit costDone ();

                            break;
                        case 2://火车票
                            busiType =true;
                            player->stop();
                            this->sendPlayText("火车票识别成功");
                            bill.setBilltype("火车票");
                            type = "火车票";
                            bill.setBilltype(type);

                            if(infoObj.contains("person")){
                                QJsonValue personVal = infoObj.value("person");
                                if(personVal.isString()){
                                    user = personVal.toString();
                                    trainPerson =user;
                                    bill.setBilluser(user);
                                }
                            }
                            if(infoObj.contains("date")){
                                QJsonValue dateVal = infoObj.value("date");
                                if(dateVal.isString()){
                                    billDate = dateVal.toString();
                                    trainDate = billDate;
                                    bill.setBilldate(billDate);
                                }
                            }
                            if(infoObj.contains("departure")){
                                QJsonValue departureVal = infoObj.value("departure");
                                if(departureVal.isString()){
                                    start = departureVal.toString();
                                    trainStart = start;
                                    bill.setBillstartplace(start);
                                }
                            }
                            if(infoObj.contains("price")){
                                QJsonValue priceVal = infoObj.value("price");
                                if(priceVal.isString()){
                                    money = priceVal.toString();
                                    trainPrice = money;
                                    bill.setBillmoney(money);
                                }
                            }
                            if(infoObj.contains("destination")){
                                QJsonValue destinationVal = infoObj.value("destination");
                                if(destinationVal.isString()){
                                    end = destinationVal.toString();
                                    trainEnd = end;
                                    bill.setBillendplace(end);
                                }
                            }
                            emit busiDone ();
                            break;
                        case 3://飞机票
                            abroadType = true;
                            player->stop();
                            this->sendPlayText("飞机票识别成功");
                            type = "飞机票";
                            bill.setBilltype(type);

                            if(infoObj.contains("person")){
                                QJsonValue personVal = infoObj.value("person");
                                if(personVal.isString()){
                                    user = personVal.toString();
                                    bill.setBilluser(user);
                                }
                            }
                            if(infoObj.contains("date")){
                                QJsonValue dateVal = infoObj.value("date");
                                if(dateVal.isString()){
                                    billDate = dateVal.toString();
                                    bill.setBilldate(billDate);
                                }
                            }
                            if(infoObj.contains("departure")){
                                QJsonValue departureVal = infoObj.value("departure");
                                if(departureVal.isString()){
                                    start = departureVal.toString();
                                    bill.setBillstartplace(start);
                                }
                            }
                            if(infoObj.contains("price")){
                                QJsonValue priceVal = infoObj.value("price");
                                if(priceVal.isString()){
                                    money = priceVal.toString();
                                    bill.setBillmoney(money);
                                }
                            }
                            if(infoObj.contains("destination")){
                                QJsonValue destinationVal = infoObj.value("destination");
                                if(destinationVal.isString()){
                                    end = destinationVal.toString();
                                    bill.setBillendplace(end);
                                }
                            }
                            emit abroadDone ();

                            break;
                        case 4://出租车票
                            player->stop();
                            this->sendPlayText("出租车票识别成功");
                            type = "出租车票";
                            bill.setBilltype(type);

                        default:
                            break;
                        }

                        //添加一行数据
                        table = ui->billTable;
                        int rowIndex = table->rowCount(); //获取行号
                        table->setRowCount(rowIndex + 1);
                        table->setRowHeight(rowIndex, 24);//设置行的高度
                        QTableWidgetItem *item = new QTableWidgetItem (type);
                        table->setItem(rowIndex, 0, item);
                        item = new QTableWidgetItem (bill.getBillcontent());
                        //                        item = new QTableWidgetItem (billContent);
                        table->setItem(rowIndex, 1, item);
                        item->setIcon(QIcon("/home/intelligence/qt/see.png"));
                        //在Table中显示上传的票据图片
                        QLabel *label = new QLabel(this);
                        pixmap.scaled(label->size(),Qt::KeepAspectRatio);
                        label->setScaledContents(true);
                        label->setPixmap(pixmap);
                        table->setCellWidget(rowIndex, 1, label);

                        //                        item = new QTableWidgetItem (billDate);
                        table->setItem(rowIndex, 1, item);
                        item = new QTableWidgetItem (bill.getBilldate());

                        table->setItem(rowIndex, 2, item);
                        item = new QTableWidgetItem (user);
                        //                        item = new QTableWidgetItem (money);

                        table->setItem(rowIndex, 3, item);
                        item = new QTableWidgetItem (bill.getBillmoney());

                        table->setItem(rowIndex, 4, item);
                        item = new QTableWidgetItem (start);

                        table->setItem(rowIndex, 5, item);
                        item = new QTableWidgetItem (end);
                        //                        item = new QTableWidgetItem (other);

                        table->setItem(rowIndex, 6, item);
                        item = new QTableWidgetItem (bill.getBillother());

                        table->setItem(rowIndex, 7, item);
                        item = new QTableWidgetItem (bill.getBillnumber());

                        table->setItem(rowIndex, 8, item);
                        item = new QTableWidgetItem (bill.getBillcode());

                        table->setItem(rowIndex, 9, item);
                        item = new QTableWidgetItem (bill.getBillcheckcode());

                        connect(table,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(showBillimg(QTableWidgetItem*)));

                    }
                }
            }
        }

    }
    //    QString all = reply->readAll();
    //    qDebug()<<endl<<all<<endl;
    //    ui->billText->setText(all);
    //QTableWidget * table = ui->billTable;

}

/**
* @brief      查看电子版票据
* @author        ljh
* @date        2018-09-12
*/
void MainWindow::showBillimg(QTableWidgetItem *img)
{
    billInfoDialog = new billinfodialog(this);

    int index=img->row();
    if(img->column()==1){
        billInfoDialog->showbill(blpath.at(index));
        billInfoDialog->show();
    }
}

/**
* @brief      语音识别返回处理
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::voiceReply(QNetworkReply * reply){
    //    qDebug()<<"语音识别返回结果处理:"<< reply->error() << endl;

    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        //        qDebug()<<"开始识别语音："<<all<<endl;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        qDebug()<<"识别语音："<<QString(doucment.toJson()).replace("\n","").replace("\"","")<<endl;
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {//JSON转换未出错
            if(doucment.isObject()){ // JSON 文档为对象
                QJsonObject object = doucment.object();
                int statusCode = 0;//定义statusCode
                if(object.contains("statusCode")){
                    QJsonValue statusCodeVal = object.value("statusCode");
                    statusCode = statusCodeVal.toString().toInt();
                    qDebug() << "statusCode:" << statusCode << endl;
                }

                //如果statusCode == 1,识别成功，获取指令编码和识别字符串
                if(statusCode == 1){
                    QString instruction_number;
                    QString resultStr;
                    //获取指令编码
                    if(object.contains("instruction_number")){
                        QJsonValue instructionVal = object.value("instruction_number");
                        if(instructionVal.isString()){
                            instruction_number = instructionVal.toString().trimmed();
                            qDebug() << "1111111"<<instruction_number;
                        }
                    }
                    qDebug()<<"instruction_number:"<<instruction_number<<endl;
                    //获取识别字符串并拼接
                    if(object.contains("content")){
                        QJsonValue contentVal = object.value("content");
                        if(contentVal.isObject()){
                            QJsonObject contentObj = contentVal.toObject();
                            if(contentObj.contains("result")){
                                QJsonValue resultsVal = contentObj.value("result");
                                if(resultsVal.isArray()){
                                    QJsonArray resultArray =resultsVal.toArray();
                                    int size = resultArray.size();
                                    for(int i = 0; i < size ; i++){
                                        QJsonValue resultVal = resultArray.at(i);
                                        if(resultVal.isString()){
                                            //                                            QString resultStrT = resultVal.toString().trimmed();
                                            //去掉逗号
                                            resultStr = resultVal.toString().trimmed();

                                            qDebug() << "resultStrT:" << resultStr << endl;
                                            //                                            resultStrT.replace(resultStrT.length()-1,1,"");
                                            //                                            resultStr.append(resultStrT);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    qDebug()<<"返回结果："<<resultStr<<endl;
                    this->instructionExp(instruction_number);
                }

                else
                {
                    qDebug()<<"语音识别失败:"<<endl;

                }
            }
        }
    }
}

/**
* @brief       语音合成结果处理
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::playReply(QNetworkReply * reply){
    if(reply->error() ==    QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        //        qDebug()<<"语音合成输出：";
        //        qDebug()<<"语音合成输出："<<all<<endl;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
            if(doucment.isObject()){
                QJsonObject object = doucment.object();
                int statusCode = 0;
                if(object.contains("statusCode")){
                    QJsonValue statusVal = object.value("statusCode");
                    if(statusVal.isString()){
                        statusCode = statusVal.toString().toInt();
                    }
                }
                qDebug()<<statusCode<<endl;
                if(statusCode == 1){
                    if(object.contains("content")){
                        QJsonValue contentVal = object.value("content");
                        QByteArray playData;
                        if(contentVal.isString()){
                            playData = contentVal.toString().toLocal8Bit();
                            playData = playData.replace("-","+");
                            playData = playData.replace("_","/");
                            playData = QByteArray::fromBase64(playData);

                            QString baseDir = QCoreApplication::applicationDirPath();
                            QString fileDir = baseDir.append("/files/player/");

                            //文件夹不存在的时候创建
                            QDir dir;
                            if(!dir.exists(fileDir))
                            {
                                bool res = dir.mkpath(fileDir);
                                qDebug() << "创建目录结果:" << res;
                            }

                            fileDir.append(commonutils.getCurrentTime());
                            fileDir.append(".wav");
                            QFile file(fileDir);
                            file.open(QIODevice::WriteOnly);
                            QDataStream out(&file);
                            out<<playData;
                            file.close();
                            //播放前，需停止语音识别功能
                            audioRecorder->stop();
                            player->setMedia(QUrl::fromLocalFile(fileDir));
                            player->play();
                            playStatus = true;
                        }
                    }
                }
            }
        }
    }
}


//-------------------------------------------slots end----------------------------------------



//-------------------------------------------function start----------------------------------------

/**
* @brief         设置录音机
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::setRecord(){
    audioRecorder->setAudioInput(audioRecorder->audioInputs().at(0));
    audioRecorder->setContainerFormat("wav");
    QAudioEncoderSettings settings;// 音频编码设置
    settings.setCodec("audio/PCM");  //编码
    settings.setSampleRate(16000); //采样率
    settings.setBitRate(128000); //比特率
    settings.setChannelCount(1); //通道数
    settings.setQuality(QMultimedia::VeryHighQuality); //品质
    settings.setEncodingMode(QMultimedia::ConstantBitRateEncoding); //编码模式
    QString container = "wav";
    //    QString container = "audio/x-wav";

    //待解决
    audioRecorder->setAudioSettings (settings);
    //audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);
}

/**
* @brief         开始录音
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::startRecord(){
    QString baseDir = QCoreApplication::applicationDirPath();
    QString fileDir = baseDir.append("/files/records/");

    //创建目录
    QDir dir;
    if(!dir.exists(fileDir))
    {
        bool res = dir.mkpath(fileDir);
        qDebug() << "录音保存目录:" << res;
    }

    fileDir.append(commonutils.getCurrentTime());
    fileDir.append(".wav");
    audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileDir));
    audioRecorder->record();
}

/**
* @brief         结束录音
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::stopRecord(){
    audioRecorder->stop();
    QString fileDir = audioRecorder->outputLocation().toLocalFile();
    qDebug()<<endl<<audioRecorder->outputLocation().toLocalFile()<<endl;
    QFile file(fileDir);
    //发送语音指令，进行识别
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(voiceReply(QNetworkReply*)));

    if (!file.open(QIODevice::ReadOnly)||file.size()==0)
    {
        file.close();
        return ;
    }
    QByteArray fdata = file.readAll();
    file.close();
    fdata = fdata.toBase64();
    fdata = fdata.replace("+","-");
    fdata = fdata.replace("/","_");

    QUrlQuery params;

    params.addQueryItem("audio",fdata);
    params.addQueryItem("audioType",".wav");
    QString  data = params.toString();

    this->printFile("voice.txt",fdata);
    //    QString address = HOST.append("/understand/recognition/");

    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/understand/recognition/"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->post(request,params.toString().toUtf8());

    recordNum = 0;
    recordHigh = 0;
    recordLow = 0;
    recordSend = false;
    this->startRecord();
}



/**
* @brief        语音识别指令处理
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::instructionExp(QString instruction){
    QString instru = instruction.left(4);
    QString returnWords = instruction.mid(7);

    qDebug()<<"instru:"<<instru<<endl;
    qDebug()<<"returnWords:"<<returnWords<<endl;

    //    0005###请填写出国报销单
    //    0006###报销已确认
    //    0007###报销已取消
    //    0008###报销已提交
    //    0010###请重新选择报销单种类
    //    0011###返回上一步
    //    0012###执行下一步
    //    1000###指令不识别，只输入正确指令
    //    0020###请选择登录方式
    //    0021###请讲身份证放在指定位置
    //    0022###请将脸部对准摄像头
    //    0023###请输入账号
    //    0024###登录已取消
    //    0025###正在登录
    //    0026###登录已结束
    //    2000###指令不识别，只输入正确指令
    //    0030###开始注册流程
    //        0031###请输入账号密码
    //        0032###请将脸部对准摄像头
    //        0033###请讲身份证放在指定位置
    //        0034###已跳过身份认证
    //        0035###已注册账号
    //        0036###注册已结束
    //        3000###指令不识别，请输入正确指令
    //        9999###我不知道你在说什么!

    if(VerifyNumber(instru))
    {
        //清空系统状态  待定
        this->on_firstBtn_clicked();

        player->stop();
        this->sendPlayText(returnWords.toUtf8());
        if("0001"==instru){//我?要?报销     //    0001###请选择验证方式
            ui->mainWidget->setCurrentIndex (2);
            //            ui->nextStepBtn->clicked();
        }else if ("0003"==instru){//人脸(识别)?(验证)   //    0003###请将脸部对准摄像头

            this->on_faceBtn_clicked();

        }else if("0002"==instru){//身份证(识别)?(验证)     //    0002###请将身份证放在指定位置
            //            this->on_faceBtn_clicked();
            this->on_idBtn_clicked();

        }/*else if(""==instru && 1==currentIndex){//缺少帐号登录指令
            ui->accountBtn->clicked();
        }*/
        else if("0004"==instru){//差旅(报销单)     //    0004###请填写差旅报销单
            //            ui->busiBtn->clicked();
            this->on_busiBtn_clicked();
        }else if ("0005"==instru){//出国(报销单)
            this->on_abroadBtn_clicked();
        }else if(""==instru && 9==currentIndex){//缺少费用报销单指令
            ui->busiBtn->clicked();
        }else if("0011"==instru ){//[上一步|返回]
            this->toLastStep();
        }else if("0012"==instru){//[下一步|返回]
            this->toNextStep();
        }else if (("0006"==instru ||"0008"==instru)&& 15==currentIndex){//确认/提交(报销)
            ui->confirmBtn->clicked();
        }else if ("0009"==instru && 16==currentIndex){//[退出|结束|结束报销]
            ui->logoutBtn->clicked();
        }else if ("0010"==instru && 16==currentIndex){//继续(报销)
            ui->continueExpenseBtn->clicked();
        }else if("0020"==instru && 0==currentIndex){//我?要?登录
            ui->LogBtn->clicked();
            this->sendPlayText(returnWords.toUtf8());//请选择登录方式
        }else if("0030"==instru && 0==currentIndex){//我?要?注册
            ui->RegBtn_2->clicked();
            this->sendPlayText(returnWords.toUtf8());//开始注册流程
        }else if("0021"==instru && 1==currentIndex){//身份证(登录)
            ui->idBtn->clicked();
            this->sendPlayText(returnWords.toUtf8());//请讲身份证放在指定位置
        }else if("0022"==instru && 1==currentIndex){//人脸(登录)
            ui->faceBtn->clicked();
            this->sendPlayText(returnWords.toUtf8());//请将脸部对准摄像头
        }else if("0023"==instru && 1==currentIndex){//账号(登录)
            ui->accountBtn->clicked();
            this->sendPlayText(returnWords.toUtf8());//请输入账号
        }else if("0031"==instru && 5==currentIndex){//账号(注册)

        }
    }else
    {
        player->stop();
        this->sendPlayText(instruction.toUtf8());
    }
}
/**
* @brief      音频播放状态改变时进行
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::playChanged()
{
    //    qDebug()<<"player状态："<<player->state()<<endl;
    if( QMediaPlayer::StoppedState==  player->state() && playStatus == true){
        //播放结束时，继续开启语音识别功能
        playStatus = false;
        qDebug()<<"语音录制开始"<<endl;
        recordNum = 0;
        recordHigh = 0;
        recordLow = 0;
        recordSend = false;
        this->startRecord();
    }
}

/**
* @brief         音量级别显示
* @author        胡帅成
* @date        2018-09-06
*/
template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}

// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;

    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    if (buffer.format().codec() != "audio/pcm")
        return values;

    int channelCount = buffer.format().channelCount();
    values.fill(0, channelCount);
    qreal peak_value = getPeakValue(buffer.format());
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

void MainWindow::processBuffer(const QAudioBuffer& buffer)
{
    if (audioLevels.count() != buffer.format().channelCount()) {
        qDeleteAll(audioLevels);
        audioLevels.clear();
        for (int i = 0; i < buffer.format().channelCount(); ++i) {
            QAudioLevel *level = new QAudioLevel();
            audioLevels.append(level);
            ui->levelLayout->addWidget(level);
        }
    }

    QVector<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i)
        audioLevels.at(i)->setLevel(levels.at(i));

    //    qDebug()<<"音量级别："<<levels<<endl;

    recordNum ++;
    if(levels.at(0) >= 0.35){
        recordHigh ++;
        recordLow = 0 ;
    }else{
        recordLow ++;
    }

    //    qDebug()<<"recordNum:"<<recordNum<<endl;
    if(recordNum <= 150){
        //当录音时间小于6秒时
        if(recordHigh >= 2){
            if(recordLow >= 12){
                //当1秒之内无声音时，结束录音
                recordSend = true;
                this->stopRecord();
            }
        }
    }else{
        //当录音时间大于6秒时，结束录音
        if(recordHigh >= 2){
            recordSend = true;
        }else{
            recordSend = false;
        }
        this->stopRecord();
    }
}

/**
* @brief         用户中心入口
* @author        胡帅成
* @date        2018-09-10
*/
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->userInfo)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            //            if(!loginUser.getUsername().isEmpty())
            if(loginStatus!=0)
            {
                player->stop();
                this->sendPlayText("欢迎用户"+loginUser.getUsername().toUtf8());
                userInfoDialog = new UserInfoDialog(this); //单击弹框

                //查询数据库
                if(!loginUser.getIdCard().isEmpty())
                {
                    //身份已认证
                    idCheckId = 2;
                    loginInformation = database.searchIdCardByIdNumber(loginUser.getIdCard());
                }else
                {
                    //未认证
                    idCheckId = 1;
                }
                //能弹框必然已登录
                userInfoDialog->showUserInfo(loginUser,loginInformation,loginStatus ,idCheckId);
                userInfoDialog->show();
                //监控认证页面
                connect(userInfoDialog, SIGNAL(emitIdCheck(User,int)), this, SLOT(dealGetIdCheck(User,int)));
            }else
            {
                //未登录                currentIndex = 1;
                idCheckId = 0;
            }
        }
    }
    //        return MainWindow::eventFilter(watched, event);
    //    return;
}


/**
* @brief         打印日志输出到文件
* @author        胡想成
* @date          2018-08-15
*/
void MainWindow::printFile(QString dataFileName,QByteArray fdata)
{
    //打印请求参数(测试)
    //    QString dataFileName = "reply.txt";
    QFile datafile(dataFileName);
    if(!datafile.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        QMessageBox::warning(this,"file write","can't open",QMessageBox::Yes);
    }
    QTextStream in(&datafile);
    in<<fdata<<"\n";
    datafile.close();
    qDebug()<<"--------get "+dataFileName+"-------"<<endl;
}

/**
* @brief         初始化欢迎页面
* @author        胡帅成
* @date        2018-09-11
*/
void MainWindow::paintEvent(QPaintEvent*)
{
    QString bgFilename = ":/new/prefix1/back4.jpeg";
    QPainter painter(this);
    //绘制背景
    QRect rec;
    rec.setTopLeft(QPoint(0,0));
    rec.setBottomRight(QPoint(this->width(),this->height()));
    QPixmap pix(bgFilename);//优先构造函数
    //    pix.load("../image/cheese.jpeg");
    painter.drawPixmap(rec,pix);
}

//-------------------------------------------function end----------------------------------------





/**
* @brief         跳过身份认证
* @author        胡帅成
* @date        2018-09-11
*/
void MainWindow::on_idCheckIgnoreBtn_clicked()
{
    player->stop();
    this->sendPlayText("跳过身份认证");

    this->idCardThreadStop();

    if(loginStatus==0)
    {
        currentIndex = 2;
        ui->mainWidget->setCurrentIndex(currentIndex);
    }else
    {
        //已登录状态,跳过则回到主页面
        currentIndex =9;
        ui->mainWidget->setCurrentIndex(currentIndex);
        showUserInfo (loginUser);

    }
}

/**
* @brief         身份证读取
* ni
* @author        胡帅成
* @date        2018-09-11
*/
void MainWindow::idCardRead()
{
    //局部线程的创建
    if(m_currentRunLoaclThread)
    {
        m_currentRunLoaclThread->stopImmediately();
    }
    IdCardThread *idCardThread = new IdCardThread(NULL);
    connect(idCardThread, SIGNAL(emitIdInfo(Information, int)), this, SLOT(dealIdCardReply(Information, int)),Qt::QueuedConnection);
    //识别成功则进行下一步，识别失败则返回
    connect(idCardThread,&IdCardThread::finished ,idCardThread,&QObject::deleteLater);//线程结束后调用deleteLater来销毁分配的内存
    connect(idCardThread,&QObject::destroyed,this,&MainWindow::onLocalThreadDestroy);

    idCardThread->start();
    m_currentRunLoaclThread = idCardThread;

}

void MainWindow::onLocalThreadDestroy(QObject *obj)
{
    if(qobject_cast<QObject*>(m_currentRunLoaclThread) == obj)
    {
        m_currentRunLoaclThread = NULL;
    }
}

/**
* @brief         注册时认证身份
* @author        胡帅成
* @date        2018-09-11
*/
void MainWindow::on_idCheckBtn_clicked()
{
    //判断线程状态
    if(m_currentRunLoaclThread)
    {
        m_currentRunLoaclThread->stopImmediately();
    }
    //插入身份证数据
    //封装身份注册信息
    string name = ui->idName_2->text().trimmed().toStdString();
    string birthday =ui->idBirth_2->text().trimmed().toStdString();
    string idNumber =ui->idCardNum_2->text().trimmed().toStdString();
    string address = ui->idHomeAddre_2->text().trimmed().toStdString();
    string sex =ui->idSex_2->text().trimmed().toStdString();
    string nation =ui->idNation_2->text().trimmed().toStdString();
    string signingOrganization =ui->idProvePlace_2->text().trimmed().toStdString();
    string startDay =ui->idStartDay_2->text().trimmed().toStdString();
    string endDay =ui->idEndDay_2->text().trimmed().toStdString();
    string newAdd =ui->idNewestAddre_2->text().trimmed().toStdString();

    qDebug() << "------regInformation sex:------"<<ui->idSex_2->text().trimmed()<< endl;
    qDebug() << "------regInformation nation:------"<<ui->idNation_2->text().trimmed()<< endl;


    //插入身份证数据  认证之前先判断是否已存在此用户
    if(!database.CheckUserByIdCard(QString::fromStdString(idCardInformation.getIdNumber())))
    {
        //更新身份信息
        idCardInformation.setName(name);
        idCardInformation.setSex(ui->idSex_2->text().trimmed().toStdString());
        idCardInformation.setNation(nation);
        idCardInformation.setSex(sex);
        idCardInformation.setBirthday(birthday);
        idCardInformation.setAddress(address);
        idCardInformation.setSigningOrganization(signingOrganization);
        idCardInformation.setIdNumber(idNumber);
        idCardInformation.setValidityPeriodStart(startDay);
        idCardInformation.setValidityPeriodEnd(endDay);
        idCardInformation.setNewestAddress(newAdd);
        //更新注册账户
        regUser.setIdCard(ui->idCardNum_2->text().trimmed());
        //        regUser.setAuthoId (1);
        qDebug() << "------regUser------"<<regUser.toString()<< endl;
        qDebug() << "------regInformation------"<<idCardInformation.toString()<< endl;


        if(database.checkIdCargByIdNumber(QString::fromStdString(idCardInformation.getIdNumber())))
        {
            if(database.updateIdCard(idCardInformation))
            {
                //插入idcard表，如果已经存在，就更新
                if(database.updateUser(regUser))
                {
                    //更新user表字段
                    regCode = 3;
                    QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("身份信息更新成功!"));

                    player->stop();
                    this->sendPlayText("身份信息更新成功");

                    if(loginStatus==0)
                    {
                        currentIndex = 2;
                        ui->mainWidget->setCurrentIndex(currentIndex);
                    }
                    else
                    {
                        currentIndex =9;
                        ui->mainWidget->setCurrentIndex(currentIndex);
                        //需要更新一下user
                        this->showUserInfo(loginUser);

                    }

                }else
                {
                    regCode = 2;
                    player->stop();
                    this->sendPlayText("身份信息更新失败");
                    QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("身份信息更新失败!"));
                    currentIndex = 2;
                    ui->mainWidget->setCurrentIndex(currentIndex);
                }
            }
            else
            {
                regCode = 2;
                player->stop();
                this->sendPlayText("身份信息更新失败");
                QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("身份信息更新失败!"));
                currentIndex = 2;
                ui->mainWidget->setCurrentIndex(currentIndex);
            }
        }
        else
        {
            //如果不存在，则插入idcard表并更新user的idcard字段
            if(database.insertIdCard(idCardInformation) && database.updateUser (regUser))
            {
                idCheckId = 1;
                regCode = 3;
                player->stop();
                this->sendPlayText("身份证认证成功");
                QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("身份证认证成功!"));

                //如果未登录,则为注册用户
                if(loginStatus==0)
                {
                    currentIndex = 2;
                    ui->mainWidget->setCurrentIndex(currentIndex);
                }else
                {
                    //已登录用户,认证身份证后返回
                    isIdcardChecked = true;//身份已认证
                    currentIndex = 9;
                    ui->mainWidget->setCurrentIndex(currentIndex);
                    this->showUserInfo(loginUser);
                }

            }else
            {
                regCode = 2;
                player->stop();
                this->sendPlayText("身份证认证失败");
                QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("身份证认证失败!"));

                //如果未登录,则为注册用户
                if(loginStatus==0)
                {
                    currentIndex = 2;
                    ui->mainWidget->setCurrentIndex(currentIndex);
                }else
                {
                    //已登录用户,直接返回
                    currentIndex = 9;
                    ui->mainWidget->setCurrentIndex(currentIndex);
                    this->showUserInfo(loginUser);
                }
            }
        }
    }
    else
    {
        regCode=2;
        player->stop();
        this->sendPlayText("身份证信息已注册");
        QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("此身份证信息已存在!"));

        this->idCardThreadStop();
        //判断登录状态
        if(loginStatus == 0)
        {
            currentIndex =2;
            ui->mainWidget->setCurrentIndex(2);
            player->stop();
            this->sendPlayText("账号注册完成");
        }else
        {
            //            已登录状态
            currentIndex = 9 ;
            ui->mainWidget->setCurrentIndex(currentIndex);
            showUserInfo (loginUser);

        }

    }

}/*
 }*/


/**
* @brief         人脸注册
* @author        胡帅成
* @date        2018-09-06
*/
//void MainWindow::on_facePreBtn_clicked()
//{
//    //设置取景器
//    viewfinder = new QCameraViewfinder(this);
//    ui->faceRegImageLayout->addWidget(viewfinder);
//    camera->setViewfinder(viewfinder);
//    viewfinder->resize(600,400);

//    imageCaptureReg = new QCameraImageCapture(camera);
//    camera->setCaptureMode(QCamera::CaptureStillImage);
//    imageCaptureReg->setCaptureDestination(QCameraImageCapture::CaptureToFile);

//    camera->searchAndLock();
//    camera->start();
//    camera->unlock();

//    //跳到人脸注册页面
//    currentIndex = 7;
//    ui->mainWidget->setCurrentIndex(currentIndex);
//}

void MainWindow::showUserInfo(User user)
{
    //    if(!user.getUsername().isEmpty())
    if(loginStatus!=0)
    {
        qDebug()<<"用户已登陆"<<endl;
        ui->userInfo->show();
        ui->userInfo->setText("欢迎用户："+user.getUsername());
        ui->faceManBtn->show();//人脸库管理登陆后才现实
    }else
    {
        ui->userInfo->hide();
        ui->faceManBtn->hide();
    }
}
/**
* @brief         终止后台身份证读取线程
* @author        胡帅成
* @date        2018-09-13
*/
void MainWindow::idCardThreadStop()
{
    qDebug() << "------销毁身份证识别线程："<< endl;

    //局部线程的创建
    if(m_currentRunLoaclThread)
    {
        m_currentRunLoaclThread->stopImmediately();
    }
}

/**
* @brief         注册页图片显示
* @author        胡帅成
* @date        2018-09-17
*/
void MainWindow::showRegPhoto(QString filePath)
{
    qDebug() << "路径:"<< filePath;
    if(!filePath.isEmpty())
    {
        QPixmap avater;
        avater.load(filePath);

        QLabel* avaterRegLabel = ui->mainWidget->findChild<QLabel*>("avaterRegLabel"); //根据子控件的名称查找子控件
        avaterRegLabel->setPixmap(avater);
        //        ui->avaterRegLabel2->setPixmap(avater);
    }


}


void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << "选择文件";
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("打开图片"));
    //设置默认文件路径    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Images(*.jpg *.jpeg)"));
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(fileDialog->exec()){
        fileNames = fileDialog->selectedFiles();
    }
    //    QString fileName;
    avaterFilePath = fileNames.at(0);
    qDebug() <<"avater avaterFilePath: "<< avaterFilePath;
    ui->avaterLineEdit->setText(avaterFilePath);

    if(!avaterFilePath.isEmpty())
    {
        //判断路径的合法性
        //        QFile file(avaterFilePath);
        QFile* file= new QFile(avaterFilePath); //file为二进制数据文件名

        if (!file->open(QIODevice::ReadOnly))
        {
            //            file.close();
            QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("图片打开失败!"));
            ui->avaterLineEdit->clear();
            ui->avaterRegBoxLabel->clear();
            //            qDebug() << "图片打开失败";
            //            currentIndex = 5;
            //            ui->mainWidget->setCurrentIndex(currentIndex);
            //            ui->avaterLineEdit->clear();
        }else
        {
            //插入头像
            QByteArray data;
            //            file.open(QIODevice::ReadOnly);
            data = file->readAll();
            QVariant imageData(data);
            regUser.setAvater(imageData);

            QPixmap avater;
            avater.loadFromData(data,"jpg");
            ui->avaterRegBoxLabel->setPixmap(avater);
            ui->avaterRegBoxLabel->setScaledContents(true);
        }
        file->close();
    }
}

void MainWindow::dealGetIdCheck(User user, int currentIndex)
{
    qDebug() << "开始认证";
    regUser = user;
    idCheckId = 1;
    toCurrentPage (7);
}

/**
* @brief         首页
* @author        胡帅成
* @date        2018-10-11
*/
void MainWindow::on_firstBtn_clicked()
{
//    currentIndex=0;
//    ui->mainWidget->setCurrentIndex(currentIndex);
    //清空loginUser
    loginStatus = 0;
    //    loginUser.setUsername("");
    User user;
    loginUser = user;
    qDebug() << "清空loginUser:" <<loginUser.toString () <<endl;
    this->showUserInfo(loginUser);
    loginStatus=0;
    // 清空idinformation
    Information information;
    idCardInformation = information;
    //资源回收释放.

    switch (currentIndex) {
    case 3: //人脸识别页面
        qDebug() << "test11111111";
        player->stop();
        this->sendPlayText("人脸登录停止,返回首页");
        this->deleteFace ();
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    case 5: //身份证登录页面
        player->stop();
        this->sendPlayText("身份证识别停止,返回首页");
        this->idCardThreadStop ();
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    case 4 : //登录页面
        player->stop();
        this->sendPlayText("登录停止停止,返回首页");
        this->deleteAccountInfo (); //清空账号登录信息.
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    case 6 : //用户名和密码注册页面
        player->stop();
        this->sendPlayText("停止注册,返回首页");
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        this->deleteAccountReg ();
        break;
    case 7 : //人脸注册页面
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("正在人脸注册,请勿退出"));
        break;
    case 8 : //身份证识别页面
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("正在身份识别,请勿退出"));
        break;
    default:
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    }

}
/**
* @brief         人脸库管理
* @author        胡帅成
* @date        2018-10-12
*/
void MainWindow::on_faceManBtn_clicked()
{
    //查询人脸库列表
    faceMangeDialog = new FaceMangeDialog(this);
    faceMangeDialog->show();
}

/**
* @brief       扫描票据
* @author      胡坤伦
* @date        2019-03-07
*/
void MainWindow::on_scanBillbtn_clicked()
{
    costRowcount = 1;
    scanInfoDialog = new scanDialog(this);
    scanInfoDialog->show ();
    connect (scanInfoDialog,&scanDialog::scanDone,this,&MainWindow::billScanRelpy);
}
/**
* @brief       给后端发送票据信息
* @author      胡坤伦
* @date        2019-03-07
*/

void MainWindow::billScanRelpy ()
{
        qDebug () <<"111";
        QString runpath= QCoreApplication::applicationDirPath();
        scanBillpath=runpath+"/finalbill.jpg";

        qDebug()<<"runpath:"<<runpath;
        qDebug() << "scanBillpath:" << scanBillpath;

        manager = new QNetworkAccessManager(this);
        //读取完成后识别票据
        QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(billReply(QNetworkReply*)));

        QFile file(scanBillpath);
        if (!file.open(QIODevice::ReadOnly)||file.size()==0)
        {
            file.close();
            return ;
        }
        QByteArray fdata = file.readAll();
        file.close();
        fdata = fdata.toBase64();
        fdata = fdata.replace("+","-");
        fdata = fdata.replace("/","_");

        QUrlQuery params;
        params.addQueryItem("image",fdata);
        params.addQueryItem("imageType",".jpg");
        QString  data = params.toString();

        //打印票据图片编码
        this->printFile("billReply.txt", fdata);
        //    QString address = HOST.append("/bill/");

        QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/bill/"));
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
        manager->post(request,params.toString().toUtf8());
//        scanInfoDialog->close ();
}


bool MainWindow::VerifyNumber(QString str) { //这一步做什么,
    std::string temp = str.toStdString();
    for (int i = 0; i < str.length(); i++)
    {
        if (temp[i]<'0' || temp[i]>'9')
        { return false;
        }
    }
    return true;
}

void MainWindow::on_carmSkipBtn_clicked()
{
    idCheckId = 1;//认证的时候调用
    //    currentIndex=4;
    //    ui->mainWidget->setCurrentIndex(currentIndex);
    //    this->idCardRead();
    toCurrentPage (7);
}

//void MainWindow::progress(int val)
//{
//    ui->progressBar->setValue(val);
//}

//void MainWindow::receiveMessage(const QString &str)
//{
//    //    ui->textBrowser->append(str);
//}

//void MainWindow::heartTimeOut()
//{
//    static int s_heartCount = 0;
//    ++s_heartCount;
//    if(s_heartCount > 100)
//    {
//        s_heartCount = 0;
//    }

//    //    ui->progressBar_heart->setValue(s_heartCount);
//}
////创建线程
//void MainWindow::startNetWorkThread()
//{
//    if(m_objThread)
//    {
//        return;
//    }
//    m_objThread= new QThread();
//    m_obj = new NetworkThread();
//    m_obj->moveToThread(m_objThread);
//    connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
//    connect(m_objThread,&QThread::finished,m_obj,&QObject::deleteLater);
//    connect(this,&MainWindow::startNetWorkThreadWork1,m_obj,&NetworkThread::runNetwork);
//    connect(this,&MainWindow::startNetWorThreadWork2,m_obj,&NetworkThread::runNetwork2);

//    //    connect(this,&MainWindow::startfaceCheckThreadWork,m_obj,&NetworkThread::runfaceCheckNetwork);

//    connect(m_obj,&NetworkThread::progress,this,&MainWindow::progress);
//    connect(m_obj,&NetworkThread::message,this,&MainWindow::receiveMessage);

//    m_objThread->start();
//}
//创建线程
//void MainWindow::startfaceCheckThread()
//{
//    qDebug()<<"startfaceCheckThread:" <<endl;

//    if(m_objThread)
//    {
//        return;
//    }
//    m_objThread= new QThread();
//    m_obj = new NetworkThread();
//    m_obj->moveToThread(m_objThread);
//    connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
//    connect(m_objThread,&QThread::finished,m_obj,&QObject::deleteLater);

//    connect(this,&MainWindow::startfaceCheckThreadWork,m_obj,&NetworkThread::runfaceCheckNetwork);
//    connect(this,&MainWindow::startfaceCheckThreadWork,m_obj,&NetworkThread::runNetwork);

//    connect(m_obj,&NetworkThread::progress,this,&MainWindow::progress);
//    connect(m_obj,&NetworkThread::emitCode,this,&MainWindow::faceCheckResult);
//    connect(m_obj,&NetworkThread::message,this,&MainWindow::receiveMessage);

//    m_objThread->start();
//}

//void MainWindow::on_faceCheckBtn_clicked()
//{
//    //设置取景器
//    viewfinder = new QCameraViewfinder(this);
//    ui->imageLayout->addWidget(viewfinder);
//    camera->setViewfinder(viewfinder);
//    viewfinder->resize(600,350);

//    imageCapture = new QCameraImageCapture(camera);
//    camera->setCaptureMode(QCamera::CaptureStillImage);
//    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);

//    //扫描人脸次数
//    facetime = 0;
//    currentIndex = 2;
//    ui->mainWidget->setCurrentIndex(currentIndex);
//    camera->start();

//    QThread* backgroundRecvThread = new QThread;
//    NetworkHandler *handler = new NetworkHandler(NULL);
//    handler->moveToThread(backgroundRecvThread);

//    if(QCamera::ActiveStatus == camera->status()){
//        connect(handler, &NetworkHandler::updateCarmer,this, &MainWindow::receiveCarmer,Qt::QueuedConnection);
//        connect(handler, &NetworkHandler::emitCode,this, &MainWindow::faceCheckResult,Qt::QueuedConnection);

//    }

//    backgroundRecvThread->start();



//}

//void MainWindow::receiveCarmer(int id)
//{
//    //update ui
//    imageCapture->capture();
//    camera->stop();
//    if(id == 2)
//    {
//        idFace =2;
//        connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), m_network, SLOT(sendPhoto(int,QImage)));
//    }
//}


//调用线程的中断
//void MainWindow::on_pushButton_qobjectStop_clicked()
//{
//    if(m_objThread)
//    {
//        if(m_obj)
//        {
//            m_obj->stop();
//        }
//    }
//}

//void MainWindow::faceCheckResult(int code, QMap<QString, QString> map)
//{
//    QString str = map["info"];
//    qDebug() <<" map['info']:"<<str<<endl;
//    camera->stop();

//    if(code ==1)
//    {
//        player->stop();
//        this->sendPlayText("登录成功");
//        QMessageBox::information(NULL, "登录成功", str, QMessageBox::Yes, QMessageBox::Yes);

//        QString username = map["username"];
//        if(!username.isEmpty())
//        {
//            loginUser = database.SearchUserByUsername(username);
//        }
//        this->showUserInfo(loginUser);

//        currentIndex = 8;
//        ui->mainWidget->setCurrentIndex(currentIndex);
//    }else if(code ==2)
//    {
//        //数据库不存在此人脸信息
//        //                                    player->stop();
//        //                                    this->sendPlayText("人脸不存在，请重新注册！");
//        QMessageBox::critical(NULL, "警告", str, QMessageBox::Yes, QMessageBox::Yes);

//        currentIndex = 0;
//        ui->mainWidget->setCurrentIndex(currentIndex);
//    }
//    else if(code == -1)
//    {
//        qDebug() <<"faceCheckResult fail2"<<endl;
//        QMessageBox::critical(NULL, "警告", str, QMessageBox::Yes, QMessageBox::Yes);
//        currentIndex = 0;
//        ui->mainWidget->setCurrentIndex(currentIndex);
//    }
//    else
//    {
//        qDebug() <<"fail"<<endl;
//    }
//}


void MainWindow::on_talkBtn_clicked()
{
    qmlRegisterType<Turing>("io.qt.turing", 1, 0, "Turning");

    QQuickView *view = new QQuickView();
    QWidget *widget = QWidget::createWindowContainer(view, this);
    //    connect(turing,&Turing::received,this,&MainWindow::turingSpeak);//turing speak
    view->setSource(QUrl("qrc:/main.qml"));
}



//void MainWindow::on_answerBtn_clicked()
//{

//}

void MainWindow::on_expenseBtn_clicked()
{
    toCurrentPage (1);
}

void MainWindow::on_applyButton_clicked()
{
    currentIndex = 10;
    ui->mainWidget->setCurrentIndex (currentIndex);
}

void MainWindow::on_advisoryButton_clicked()
{

    this->on_talkBtn_clicked ();
}

void MainWindow::on_costBaseButton_clicked()
{
    currentIndex = 13;
    ui->mainWidget->setCurrentIndex (currentIndex);
}

void MainWindow::on_costScanButton_clicked()
{
    currentIndex = 11;
    ui->mainWidget->setCurrentIndex (currentIndex);
}

void MainWindow::on_costItemButton_clicked()
{
    currentIndex = 18;
    ui->mainWidget->setCurrentIndex (currentIndex);
}

void MainWindow::on_costPayButton_clicked()
{
    currentIndex = 19;
    ui->mainWidget->setCurrentIndex (currentIndex);
}
/**
* @brief       显示报销流程按钮
* @author      胡坤伦
* @date        2019-04-01
*/
void MainWindow::isShowguiInforn ()
{
    ui->costBaseButton->show ();
    ui->costBaseLabel->show();
    ui->costScanButton->show ();
    ui->costScanLabel->show ();
    ui->costItemButton->show();
    ui->costItemLabel->show();
    ui->costPayButton->show();
    ui->costPayLabel->show ();
}
/**
* @brief       隐藏报销流程按钮
* @author      胡坤伦
* @date        2019-04-01
*/
void MainWindow::noShowguiInforn ()
{
    ui->costBaseButton->hide ();
    ui->costBaseLabel->hide ();
    ui->costScanButton->hide ();
    ui->costScanLabel->hide ();
    ui->costItemButton->hide ();
    ui->costItemLabel->hide ();
    ui->costPayButton->hide ();
    ui->costPayLabel->hide ();
}

void MainWindow::on_costAgainButton_clicked()
{
    this->on_scanBillbtn_clicked();
}
