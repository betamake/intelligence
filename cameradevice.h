#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H

#include <QObject>
#include <QCameraImageCapture>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QList>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QUrlQuery>
#include "commonutils.h"
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QDir>
#include <QPixmap>
#include <QJsonArray>
#include <QFile>
#include <QObject>
#include <QList>
#include <allinterface.h>
#include<QCoreApplication>
#include <QDebug>
class Camera{
public:
    QCameraViewfinder *getviewfinder()
    {
        return this->viewfinder;
    }
    void setviewfinder(QCameraViewfinder *viewfinder)
    {
        this->viewfinder=viewfinder;
    }
    QCameraImageCapture *getimageCapture(){
        return this->imageCapture;
    }
    void setimageCapture(QCameraImageCapture *imageCapture){
        this->imageCapture=imageCapture;
    }
    QCameraImageCapture *getimageCaptureReg(){
        return this->imageCaptureReg;
    }
    void setimageCaptureReg(QCameraImageCapture *imageCaptureReg)
    {
        this->imageCaptureReg=imageCaptureReg;
    }
    QCamera *getcamera(){
        return this->camera;
    }
    void setcamera(QCamera *camera){
        this->camera=camera;
    }
    int getfaceId(){
        return this->faceId;
    }
    void setfaceId(const int faceId){
        this->faceId=faceId;
    }

private:
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
    QCameraImageCapture *imageCaptureReg;
    QCamera *camera;
    int faceId;
};

class CameraDevice : public QThread
{
    Q_OBJECT
public:
    explicit CameraDevice(QObject *parent = 0);
    ~CameraDevice();
    Camera getCameraInfo(){
        return CameraInfo;
    }
    Camera CameraInfo;
    static CameraDevice *getinstance();
    QByteArray getPixmapData(QString filePath,QImage image); //
    void run();
    void faceRegistered();

signals:
    void faceCheckDone(int sentIndex);//发送人脸检测成功信号
    void faceCheckFailure(int sentIndex);//发送人脸检测失败信号
public slots:
         void sendPhoto(int Id,QImage image);
         void faceReply(QNetworkReply * reply);
         void faceCheck();//人脸检测
         void photoRegister(int Id,QImage image);
         void faceRegReply(QNetworkReply *reply);
private:
    static CameraDevice *instance;
    int currentIndex;
    //人脸扫描
    int facetime;
    QTimer *timer;
    int sentIndex;
    int idFace;
    bool isFaceOk;
    QString address;
    CommonUtils commonutils;
};

#endif // CAMERADEVICE_H
