#include "cameradevice.h"
CameraDevice *CameraDevice::instance =NULL;
CameraDevice::CameraDevice(QObject *parent) : QThread(parent)
{
       address = "http://192.168.3.185:8000";
       this->initCamera ();

}
CameraDevice::~CameraDevice(){
    delete instance;
    instance = NULL;
}
CameraDevice *CameraDevice::getinstance ()
{
    if (NULL == instance) {
        instance = new CameraDevice();
    }
    return instance;
}
void CameraDevice::initCamera ()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if(cameras.count() > 0){
        foreach (const QCameraInfo &cameraInfo, cameras) {
            qDebug()<<cameraInfo.description();
        }
        QCamera *camera = new QCamera(cameras.at(0));
        CameraInfo.setcamera (camera);
    }
    //登录
    QCameraViewfinder *viewfinder = new QCameraViewfinder();
    CameraInfo.setviewfinder (viewfinder);
    CameraInfo.getcamera ()->setViewfinder( CameraInfo.getviewfinder ());
    CameraInfo.getviewfinder ()->resize(600,400);

    QCameraImageCapture *imageCapture = new QCameraImageCapture(CameraInfo.getcamera ());
    CameraInfo.setimageCapture (imageCapture);

    CameraInfo.getcamera ()->setCaptureMode(QCamera::CaptureStillImage);
    CameraInfo.getimageCapture ()->setCaptureDestination(QCameraImageCapture::CaptureToFile);
}
void CameraDevice::run ()
{
    isFaceOk = false;
    //扫描人脸次数
    facetime = 0;


    qDebug()<<"进入线程";
    qDebug()<<"人脸登录";
    timer = new QTimer(this);
    //    this->faceCheck ();
    timer->setInterval(2000);//1s发送一次timeout信号
    connect(timer, SIGNAL(timeout()), this, SLOT(faceCheck()));
    timer->start();//启动定时器
    exec() ;
}


QByteArray CameraDevice::getPixmapData(QString filePath,QImage image)
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
void CameraDevice::faceCheck ()
{
    qDebug()<<"第" << facetime << "次查询人脸"<<CameraInfo.getcamera ()->status();
    if(QCamera::ActiveStatus == CameraInfo.getcamera ()->status())
    {
        qDebug()<<"第" << facetime << "次查询人脸";
        facetime++;
        idFace = 2;//登录查人脸
        CameraInfo.getimageCapture ()->capture();
        //take photos
        if(facetime < 4)
        {
            qDebug()<<"第" << "facetime" << "次查询人脸";
            connect(CameraInfo.getimageCapture (), SIGNAL(imageCaptured(int,QImage)), this, SLOT(sendPhoto(int,QImage)));
        }else
        {
            qDebug() << "face check failed, stop send photos"<< endl;
            sentIndex=6;

//            QMessageBox::information(this, QString::fromUtf8("失败"),QString::fromUtf8("人脸不存在，请补录人脸信息"));
            if(timer->isActive ())
            {
                timer->stop ();
            }
            CameraInfo.getcamera ()->stop ();
            emit faceCheckFailure (sentIndex);
        }

    }
}
void CameraDevice::sendPhoto (int Id, QImage image)
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
void CameraDevice::faceReply (QNetworkReply *reply)
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
                        CameraInfo.getcamera ()->stop();
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
