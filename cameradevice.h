#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H

#include <QObject>
#include <QCameraImageCapture>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QList>
#include <QThread>
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

private:
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
    QCameraImageCapture *imageCaptureReg;
    QCamera *camera;
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
    void run();

signals:

public slots:
private:
    static CameraDevice *instance;
};

#endif // CAMERADEVICE_H
