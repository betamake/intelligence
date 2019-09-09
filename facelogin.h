#ifndef FACELOGIN_H
#define FACELOGIN_H

#include <QDialog>
#include <QCameraImageCapture>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>
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
#include <QMessageBox>
#include <QDir>
#include <QPixmap>
#include <QJsonArray>
#include <QFile>
#include <QObject>
#include <QList>
#include <allinterface.h>
#include <QElapsedTimer>
#include <cameradevice.h>
namespace Ui {
class faceLogin;
}

class faceLogin : public QDialog
{
    Q_OBJECT

public:
    explicit faceLogin(QWidget *parent = 0);
    ~faceLogin();
     void initCamera();
     void faceBegin();
     QByteArray getPixmapData(QString filePath,QImage image);
signals:
     void faceCheckDone(int sentIndex);//发送
private slots:

     void faceCheck();//人脸检测
     void sendPhoto(int Id,QImage image);
     void faceReply(QNetworkReply * reply);
//     void faceRegReply(QNetworkReply * reply);

     void on_pushButton_clicked();

     void on_RegButton_clicked();
     void photoRegister(int Id,QImage image);
     void faceRegReply(QNetworkReply *reply);
private:
    Ui::faceLogin *ui;
    //摄像头相关
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
    QCameraImageCapture *imageCaptureReg;
    QCamera *camera;
    //页面
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

#endif // FACELOGIN_H
