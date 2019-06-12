//#include "networkthread.h"
//#include <QThread>
//#include <QDebug>
//#include <QMutexLocker>
//#include <QElapsedTimer>
//#include <limits>
//#include<QNetworkAccessManager>
//#include <unistd.h>
//#include <QUrlQuery>
//#include<QJsonDocument>
//#include<QJsonParseError>
//#include <QJsonObject>
//#include<QJsonArray>
//#include<QFileDialog>
//#include<QJsonDocument>
//#include<QJsonParseError>

//#include <QtGlobal>
//#include <QFileDialog>
//#include <QDir>
//#include<QCoreApplication>


//NetworkThread::NetworkThread(QObject *parent) : QObject(parent),m_isStop(true)  ,m_runCount(10)
//  ,m_runCount2(std::numeric_limits<int>::max())
//{
//    address = "http://192.168.3.102:8000";
//}


//NetworkThread::~NetworkThread()
//{
//    qDebug() << "ThreadObject destroy";
//    emit message(QString("Destroy %1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((long)QThread::currentThreadId()));
//}

//void NetworkThread::setRunCount(int count)
//{
//    m_runCount = count;
//    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((long)QThread::currentThreadId()));
//}

//void NetworkThread::setData(QByteArray data)
//{
//    received_data = data;
//    qDebug() << "data image:" <<data.toBase64()<<endl;
//}

//void NetworkThread::receiveByteData(QByteArray data)
//{
//    qDebug() << "data image:" <<data.toBase64()<<endl;

//    received_data = data;
//}


//void NetworkThread::runfaceCheckNetwork()
//{
//    {
//        QMutexLocker locker(&m_stopMutex);
//        m_isStop = false;
//    }
//    //准备发送网络请求和接收应答
//    if(received_data.isEmpty())
//    {
//        qDebug() << "received_data image: null"<<endl;
//        return;
//    }
//    qDebug() << "received_data image: "<<received_data.toBase64()<<endl;

//    QNetworkAccessManager* faceCheckManager = new QNetworkAccessManager(this);
////    QNetworkReply *reply =
//    //发送结束后，进行人脸结果处理
//    QObject::connect(faceCheckManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceCheckReply(QNetworkReply*)));
//    QEventLoop eventLoop;
//    QObject::connect(faceCheckManager,SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));
//    eventLoop.exec();

//    //封装请求参数(看接口文档)
//    QUrlQuery params;
//    received_data = received_data.toBase64().replace("+","-").replace("/","_");
//    params.addQueryItem("image",received_data);
//    params.addQueryItem("group_id","1");
//    params.addQueryItem("imageType",".jpg");

//    QString  data = params.toString();
//    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/search/"));
//    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
//    manager->post(request,params.toString().toUtf8());
//}

//void NetworkThread::runNetwork()
//{
//    {
//        QMutexLocker locker(&m_stopMutex);
//        m_isStop = false;
//    }
//    int count = 0;
//    QString str = QString("%1->%2,thread id:%3").arg(__FILE__).arg(__FUNCTION__).arg((long)QThread::currentThreadId());
//    qDebug() << "emit str1:"<<str;

//    emit message(str);
//    int process = 0;
//    while(1)
//    {
//        {
//            QMutexLocker locker(&m_stopMutex);
//            if(m_isStop)
//                return;
//        }
//        if(m_runCount == count)
//        {
//            break;
//        }
//        sleep(1);
//        int pro = ((float)count / m_runCount) * 100;
//        if(pro != process)
//        {
//            process = pro;
//            emit progress(((float)count / m_runCount) * 100);
//            emit message(QString("Object::run times:%1,m_runCount:%2").arg(count).arg(m_runCount2));
//        }
//        ++count;
//    }
//}

//void NetworkThread::runNetwork2()
//{
//    {
//        QMutexLocker locker(&m_stopMutex);
//        m_isStop = false;
//    }
//    int count = 0;
//    QString str = QString("%1->%2,thread id:%3").arg(__FILE__).arg(__FUNCTION__).arg((long)QThread::currentThreadId());
//    emit message(str);
//    int process = 0;
//    QElapsedTimer timer;
//    timer.start();
//    while(1)
//    {
//        {
//            QMutexLocker locker(&m_stopMutex);
//            if(m_isStop)
//                return;
//        }
//        if(m_runCount2 == count)
//        {
//            break;
//        }
//        int pro = ((float)count / m_runCount2) * 100;
//        if(pro != process)
//        {
//            process = pro;
//            emit progress(pro);
//            emit message(QString("%1,%2,%3,%4")
//                         .arg(count)
//                         .arg(m_runCount2)
//                         .arg(pro)
//                         .arg(timer.elapsed()));
//            timer.restart();
//        }
//        ++count;
//    }
//}

//void NetworkThread::stop()
//{
//    QMutexLocker locker(&m_stopMutex);
//    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((long)QThread::currentThreadId()));
//    m_isStop = true;
//}
//void NetworkThread::faceCheckReply(QNetworkReply *reply){

//    if(reply->error() == QNetworkReply::NoError){
//        qDebug()<<"reply success:" <<reply->error()<<endl;

//        QByteArray all = reply->readAll();
//        QJsonParseError jsonError;
//        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
//        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
//            if(doucment.isObject()){  // JSON 文档为对象
//                QJsonObject object = doucment.object();  //  转化为对象
//                int result_num = 0;
//                if(object.contains("result_num")){
//                    QJsonValue numValue = object.value("result_num");
//                    if(numValue.isDouble()){
//                        result_num = numValue.toInt();
//                    }
//                }
//                if(result_num >=1){//人脸识别有结果
//                    if(object.contains("result")){
//                        QJsonValue resultsValue = object.value("result");
//                        if(resultsValue.isArray())
//                        {
//                            QJsonArray resultsArray = resultsValue.toArray();
//                            qDebug() << "resultsArray:" << resultsArray;

//                            QString uid;
//                            QString user_info;
//                            double score = 80;//阈值为80分
//                            int nSize = resultsArray.size();
//                            for(int i = 0  ; i < nSize ; i ++){
//                                QJsonValue resultValue = resultsArray.at(i);
//                                double scoreTemp = 0;
//                                if(resultValue.isObject()){
//                                    QJsonObject resultObj = resultValue.toObject();
//                                    //获取相似度分数
//                                    if(resultObj.contains("scores")){
//                                        QJsonValue scoresVal = resultObj.value("scores");
//                                        if(scoresVal.isArray()){
//                                            QJsonArray scoresArray = scoresVal.toArray();
//                                            QJsonValue scoreVal = scoresArray.at(0);
//                                            if(scoreVal.isDouble()){
//                                                scoreTemp = scoreVal.toDouble();
//                                                qDebug() << "scoreTemp:" << scoreTemp;
//                                            }
//                                        }
//                                    }
//                                    if(scoreTemp > score){
//                                        score = scoreTemp;
//                                        //获取唯一uid
//                                        if(resultObj.contains("uid")){
//                                            QJsonValue uidVal = resultObj.value("uid");
//                                            if(uidVal.isString()){
//                                                uid = uidVal.toString();
//                                            }
//                                        }
//                                        // 获取user_info
//                                        if(resultObj.contains("user_info")){
//                                            QJsonValue infoVal = resultObj.value("user_info");
//                                            if(infoVal.isString()){
//                                                user_info = infoVal.toString();
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                            if(score > 80){
//                                //相似度分数大于80,识别成功
//                                qDebug() << "user_info:" << user_info;
//                                qDebug() << "uid:" << uid;

//                                if(!database.CheckUserByUid(uid))
//                                {
//                                    map.insert("info","人脸不存在，请重新注册！");
//                                    emit emitCode(2, map);

//                                    //请求网络 删除人脸库数据
//                                    this->deletePhoto(uid);
//                                }else
//                                {
//                                    //成功登陆
//                                    map.insert("username",user_info);
//                                    map.insert("info","识别成功");
//                                    emit emitCode(1,map);
//                                }

//                            }else{
//                                qDebug() << "小于score:" << score;
//                                map.insert("info","相似度分数小于80,识别失败");
//                                emit emitCode(-1, map);
//                            }
//                        }
//                    }
//                }
//            }
//        }
//        else{
//            map.insert("info","识别失败");
//            emit emitCode(-1,map);
//        }
//    }
//}
///**
//* @function   sendPhoto
//* @brief         删除人脸
//* @author        胡想成
//* @date          2018-08-16
//*/
//void NetworkThread::deletePhoto(QString uid){
//    qDebug()<<"deletePhoto:"<< uid;
//    //准备发送网络请求和接收应答
//    QNetworkAccessManager* deleteManager = new QNetworkAccessManager(this);
//    //发送结束后，进行人脸结果处理
//    QObject::connect(deleteManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceDelReply(QNetworkReply*)));
////    QEventLoop eventLoop;
////    QObject::connect(deleteManager,SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));
////    eventLoop.exec();
//    //封装请求参数(看接口文档)
//    QUrlQuery params;
//    params.addQueryItem("uid", uid);
//    params.addQueryItem("group_id","1");
//    QString  data = params.toString();
//    //    QString address = HOST.append("/face/delete/");

//    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/search/"));
//    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
//    manager->post(request,params.toString().toUtf8());
//}

///**
//* @brief       人脸删除处理
//* @author        胡想成
//* @date          2018-08-16
//*/
//void NetworkThread::faceDelReply(QNetworkReply *reply){

//    qDebug()<<"----删除photo:----"<<reply->error()<<endl;
//    //拿到返回结果
//    if(reply->error() == QNetworkReply::NoError){
//        QByteArray all = reply->readAll();
//        //打印结果
//        qDebug()<<"----处理反斜杠:----"<< all <<endl;
//        QJsonParseError jsonError;
//        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
//        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
//            if(doucment.isObject()){  // JSON 文档为对象
//                QJsonObject object = doucment.object();  //  转化为对象
//                qDebug()<< "----faceDelete result:----"<<QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","") <<endl;
//                int error_code=0;
//                //删除失败
//                if(object.contains("error_code")){
//                    QJsonValue numValue = object.value("error_code");
//                    QJsonValue numValue2 = object.value("error_msg");
//                    if(numValue.isDouble()){
//                        error_code = numValue.toInt();
//                    }
//                    qDebug() <<"delete:"<< "人脸删除失败"<< numValue2<<endl;
//                }
//                //删除成功
//                else{
//                    qDebug() <<"delete:"<< "人脸删除成功"<<endl;
//                }
//            }
//        }
//    }
//}
