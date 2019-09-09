#include "cameradevice.h"
CameraDevice *CameraDevice::instance =NULL;
CameraDevice::CameraDevice(QObject *parent) : QThread(parent)
{

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
