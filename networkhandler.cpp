//#include "networkhandler.h"
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

//NetworkHandler::NetworkHandler(QObject *parent) : QObject(parent)
//{
//    address = "http://192.168.3.102:8000";

//    facetime=0;
//    m_RecvTask->setInterval(1000);
//    connect(m_RecvTask, SIGNAL(timeout()), this, SLOT(faceCheck()));
//    m_RecvTask->start();
//}
//NetworkHandler::~NetworkHandler()
//{

//}
//void NetworkHandler::faceCheck(){

//        //        QMap<QString ,QByteArray> faceParam;
//        //        faceParam.clear();
//        //        list.clear();
//        facetime++;
//        qDebug()<<"第" << facetime << "次查询人脸";
//        if(facetime > 10)
//        {
//            facetime = 0;
//            if(m_RecvTask->isActive())
//            {
//                m_RecvTask->stop();
//            }
//        }
//        //emit mainThread to update
//        emit updateCarmer(2);

//}

//void NetworkHandler::sendPhoto(int id,QImage image){


//    QByteArray fdata = getPixmapData("/files/face",image);
//    manager  = new QNetworkAccessManager();
//    //    QNetworkReply *reply =
//    //发送结束后，进行人脸结果处理
//    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceCheckReply(QNetworkReply*)));
//    QEventLoop eventLoop;
//    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));
//    eventLoop.exec();

//    //封装请求参数(看接口文档)
//    QUrlQuery params;
//    fdata = fdata.toBase64().replace("+","-").replace("/","_");
//    params.addQueryItem("image",fdata);
//    params.addQueryItem("group_id","1");
//    params.addQueryItem("imageType",".jpg");

//    QString  data = params.toString();
//    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/search/"));
//    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
//    manager->post(request,params.toString().toUtf8());
//}

//QByteArray NetworkHandler::getPixmapData(QString filePath,QImage image)
//{
//    qDebug()<<"发送照片";
//    QString baseDir = QCoreApplication::applicationDirPath();
//    QString fileDir = baseDir.append(filePath);

//    //创建目录
//    QDir dir;
//    if(!dir.exists(fileDir))
//    {
//        bool res = dir.mkpath(fileDir);
//        qDebug() << "创建人脸图片目录结果:" << res;
//    }

//    fileDir.append(commonutils.getCurrentTime());
//    fileDir.append(".jpg");

//    QPixmap pixmap = QPixmap::fromImage(image);
//    pixmap.save(fileDir);
//    qDebug()<<fileDir<<endl;

//    QFile file(fileDir);

//    if (!file.open(QIODevice::ReadOnly)||file.size()==0)
//    {
//        file.close();
//        qDebug() << "文件打开失败";
//    }
//    QByteArray fdata = file.readAll();
//    file.close();
//    return fdata;
//}

//void NetworkHandler::faceCheckReply(QNetworkReply *reply){

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
//void NetworkHandler::deletePhoto(QString uid){
//    qDebug()<<"deletePhoto:"<< uid;
//    //准备发送网络请求和接收应答
//    QNetworkAccessManager* deleteManager = new QNetworkAccessManager(this);
//    //发送结束后，进行人脸结果处理
//    QObject::connect(deleteManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceDelReply(QNetworkReply*)));
//    QEventLoop eventLoop;
//    QObject::connect(deleteManager,SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));
//    eventLoop.exec();
//    //封装请求参数(看接口文档)
//    QUrlQuery params;
//    params.addQueryItem("uid", uid);
//    params.addQueryItem("group_id","1");
//    QString  data = params.toString();

//    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/search/"));
//    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
//    manager->post(request,params.toString().toUtf8());
//}

///**
//* @brief       人脸删除处理
//* @author        胡想成
//* @date          2018-08-16
//*/
//void NetworkHandler::faceDelReply(QNetworkReply *reply){

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
//                //删除失败
//                if(object.contains("error_code")){
//                    QJsonValue numValue = object.value("error_code");
//                    QJsonValue numValue2 = object.value("error_msg");
//                    if(numValue.isDouble()){
////                        error_code = numValue.toInt();
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
