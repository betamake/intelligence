#include "facereg.h"
faceReg *faceReg::instance =NULL;
faceReg::faceReg(QObject *parent) : QThread(parent)
{
    address = "http://192.168.3.185:8000";
    this->initCamera ();
}

faceReg::~faceReg(){
    delete instance;
    instance = NULL;
}
faceReg *faceReg::getinstance ()
{
    if (NULL == instance) {
        instance = new faceReg();
    }
    return instance;
}
void faceReg::initCamera ()
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
void faceReg::run ()
{
    qDebug()<<"进入注册线程:";
    timer = new QTimer(this);
    facetime=0;
    timer->setInterval(2000);//1s发送一次timeout信号
    connect(timer, SIGNAL(timeout()), this, SLOT(faceCapute()));
    timer->start();//启动定时器
    exec() ;
}
void faceReg::faceCapute ()
{
    if(QCamera::ActiveStatus == CameraInfo.getcamera ()->status())
    {
        facetime++;
        CameraInfo.getimageCapture ()->capture();
        if(facetime < 4)
        {
            connect(CameraInfo.getimageCapture (), SIGNAL(imageCaptured(int,QImage)), this, SLOT(photoRegister(int,QImage)));
        }else
        {
            qDebug()<<"注册失败";
            if(timer->isActive ())
            {
                timer->stop ();
            }
            CameraInfo.getcamera ()->stop ();
            sentIndex = 7;
            emit faceRegFailure (sentIndex);
        }
     }
}

void faceReg::photoRegister(int id,QImage image)
{
    qDebug()<<"摄像头状态:"<<CameraInfo.getcamera ()->status();
    if(QCamera::ActiveStatus == CameraInfo.getcamera ()->status())
     {   QByteArray fdata = this->getPixmapData("/files/regFace/",image);
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
void faceReg::faceRegReply(QNetworkReply *reply){

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
                    if(numValue.isDouble()){
                    }
                    qDebug()<< "----result failure:----"<< error_msg<<endl;


                }
                //注册成功
                else{
                    //取log_id  16位字符串
                    QString log_id =  QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","").mid(8,16);
                    qDebug() <<"----result sucess:----"<<"log_id:"<<log_id<<endl;
                    CameraInfo.getcamera ()->stop();
                    if(timer->isActive ())
                    {
                        timer->stop ();
                    }
                    sentIndex=3;
                    emit faceRegSucess (sentIndex);
                }
                //清空内容并定位光标


            }
        }
    }
}
QByteArray faceReg::getPixmapData (QString filePath, QImage image)
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
