#include "facelogin.h"
#include "ui_facelogin.h"

faceLogin::faceLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::faceLogin)
{

    ui->setupUi(this);
    this->initCamera ();
    address = "http://192.168.3.185:8000";
    this->faceBegin ();

}

faceLogin::~faceLogin()
{
    delete ui;
}
void faceLogin::initCamera ()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if(cameras.count() > 0){
        foreach (const QCameraInfo &cameraInfo, cameras) {
            qDebug()<<cameraInfo.description();
        }
        camera = new QCamera(cameras.at(0));
        CameraDevice::getinstance ()->CameraInfo.setcamera (camera);
    }
   viewfinder = new QCameraViewfinder(this);
   CameraDevice::getinstance ()->CameraInfo.setviewfinder (viewfinder);
   ui->imageLayout->addWidget(CameraDevice::getinstance ()->CameraInfo.getviewfinder ());
   CameraDevice::getinstance ()->CameraInfo.getcamera ()->setViewfinder(CameraDevice::getinstance ()->CameraInfo.getviewfinder ());
   CameraDevice::getinstance ()->CameraInfo.getviewfinder ()->resize(600,400);
   //    viewfinder->show();

   imageCapture = new QCameraImageCapture(camera);
   CameraDevice::getinstance ()->CameraInfo.setimageCapture (imageCapture);
   CameraDevice::getinstance ()->CameraInfo.getcamera ()->setCaptureMode(QCamera::CaptureStillImage);
   CameraDevice::getinstance ()->CameraInfo.getimageCapture ()->setCaptureDestination(QCameraImageCapture::CaptureToFile);
   CameraDevice::getinstance ()->CameraInfo.getcamera ()->start();
}
QByteArray faceLogin::getPixmapData(QString filePath,QImage image)
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
void faceLogin::faceBegin ()
{
        isFaceOk = false;
        facetime=0;
        currentIndex=0;
        ui->stackedWidget->setCurrentIndex (currentIndex);
        timer = new QTimer(this);
        timer->setInterval(1000);//1s发送一次timeout信号
        timer->start();//启动定时器
        connect(timer, SIGNAL(timeout()), this, SLOT(faceCheck()));
}
void faceLogin::faceCheck ()
{
    if(QCamera::ActiveStatus == camera->status())
    {
        qDebug()<<"第" << facetime << "次查询人脸";
        facetime++;
        idFace = 2;//登录查人脸

        //take photos
        qDebug()<<"第" << facetime << "次查询人脸";
        if(facetime < 2)
        {
            QElapsedTimer et;
            et.start();
            imageCapture->capture();
            qDebug()<<"第" << "facetime" << "次查询人脸";
            connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(sendPhoto(int,QImage)));
            while(et.elapsed()<300)
                QCoreApplication::processEvents();

        }else
        {
            qDebug() << "face check failed, stop send photos"<< endl;
            QMessageBox::information(this, QString::fromUtf8("失败"),QString::fromUtf8("人脸不存在，请补录人脸信息"));
            if(timer->isActive ())
            {
                timer->stop ();
            }
            camera->stop ();
            currentIndex =1;
            ui->stackedWidget->setCurrentIndex (currentIndex);
        }

    }
}
void faceLogin::sendPhoto (int Id, QImage image)
{
    if(idFace == 2 && !isFaceOk)
    {
        QByteArray fdata = this->getPixmapData("/files/face",image);
        QNetworkAccessManager *manager  = new QNetworkAccessManager(this);
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
    }
}
void faceLogin::faceReply (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError) && doucment.isObject()) {
            QJsonObject object = doucment.object();
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
//                        uname = user_info;
                        isFaceOk = true;
                        camera->stop();
                        if(timer->isActive ())
                        {
                            timer->stop ();
                        }
                        sentIndex =9;
                        emit faceCheckDone (sentIndex);

                    }
                }
            }

        }
    }
}

void faceLogin::on_pushButton_clicked()
{
    currentIndex=2;
    ui->stackedWidget->setCurrentIndex (currentIndex);
    QString username = ui->usernameEdit->text ();
    QString password = ui->lineEdit_2->text ();
//    imageCaptureReg = new QCameraImageCapture(camera);
//    camera->setViewfinder(viewfinder);
//    imageCaptureReg->setCaptureDestination(QCameraImageCapture::CaptureToFile);
//    camera->start();
//    ui->imageRegLayout->addWidget (viewfinder);
    allInterface::getinstance ()->info.setusername (username);
    allInterface::getinstance ()->info.setpassword (password);
}

void faceLogin::on_RegButton_clicked()
{
//    idFace=1;
//    camera->searchAndLock();
//    imageCaptureReg->capture();
//    camera->unlock();
//    connect(imageCaptureReg, SIGNAL(imageCaptured(int,QImage)), this, SLOT(photoRegister(int,QImage)));
}
void faceLogin::photoRegister(int id,QImage image)
{
    if(idFace==1){
        QByteArray fdata = this->getPixmapData("/files/regFace/",image);
        camera->stop();//停止相机拍照

        //准备发送网络请求和接收应答
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QUrlQuery params;
        //不能直接到第二步
        QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceRegReply(QNetworkReply*)));
        fdata = fdata.toBase64();
        fdata = fdata.replace("+","-");
        fdata = fdata.replace("/","_");
        params.addQueryItem("image",fdata);
        params.addQueryItem("group_id","1");
        params.addQueryItem("uid",allInterface::getinstance ()->info.getpassword ());
        params.addQueryItem("user_info",allInterface::getinstance ()->info.getusername ());
        params.addQueryItem("imageType",".jpg");
        QString  data = params.toString();
        QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/register/"));
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
        manager->post(request,params.toString().toUtf8());
    }
}

/**
* @brief       人脸注册结果处理
* @author        胡想成
* @date          2018-08-16
*/
void faceLogin::faceRegReply(QNetworkReply *reply){

    //拿到返回结果
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
            if(doucment.isObject()){
                QJsonObject object = doucment.object();
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

                }
                //注册成功
                else{
                    //取log_id  16位字符串
                    QString log_id =  QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","").mid(8,16);
                    qDebug() <<"log_id:"<<log_id<<endl;
                }
                //清空内容并定位光标
                ui->usernameEdit->clear();
                ui->lineEdit_2->clear();
            }
        }
    }
}

