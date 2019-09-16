#ifndef FACEREG_H
#define FACEREG_H
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
class Camerainfo{
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
    QCamera *camera;
    int faceId;
};
class faceReg : public QThread
{
    Q_OBJECT
public:
    explicit faceReg(QObject *parent = 0);
    ~faceReg();
    Camerainfo getCameraInfo(){
        return CameraInfo;
    }
    Camerainfo CameraInfo;
    static faceReg *getinstance();
    QByteArray getPixmapData(QString filePath,QImage image); //
    void run();
    void initCamera();

signals:
    void faceRegSucess(int sentIndex);
    void faceRegFailure(int sentIndex);
public slots:
    void photoRegister(int Id,QImage image);
    void faceRegReply(QNetworkReply *reply);
    void faceCapute();

private:
    static faceReg *instance;
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

#endif // FACEREG_H
