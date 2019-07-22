#include "commonutils.h"
#include <QImage>
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

CommonUtils::CommonUtils(QObject *parent) : QObject(parent)
{

}

/**
* @function   getCurrentTime
* @brief         获取当前时间
* @author        胡想成
* @date          2018-08-17
*/
QString CommonUtils::getCurrentTime()
{
    QString str_time;
    QDateTime time = QDateTime::currentDateTime();
    str_time = time.toString("yyyy-MM-dd_hh-mm-ss");
    return str_time;
}


/**
* @function   getHttpRequest
* @brief         请求头处理
* @author        胡想成
* @date          2018-08-17
*/
QNetworkRequest CommonUtils::getHttpRequest(QString url){
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded;charset=utf-8");
    qDebug() <<"getHttpRequest"<<url;
    //    request.setRawHeader("Accept","text/html, application/xhtml+xml, */*");
    //    request.setRawHeader("Accept-Language","zh-CN");
    //    request.setRawHeader("X-Requested-With","XMLHttpRequest");
    //    request.setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
    //    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    //    request.setRawHeader("Accept-Encoding","gzip,deflate");
    //    request.setRawHeader("Host","localhost:8888");
    //    request.setRawHeader("Content-Length","18");
    //    request.setRawHeader("Connection","Keep-Alive");
    //    request.setRawHeader("Cache-Control","no-cache");
    return request;
}

/**
* @function   Image_To_Base64
* @brief         图片编码
* @author        胡想成
* @date          2018-08-16
*/
QByteArray  CommonUtils::Image_To_Base64(QString ImgPath){
    QImage image(ImgPath);
    QByteArray ba;
    QBuffer buf(&ba);
    image.save(&buf,"JPG",20);
    QByteArray hexed = ba.toBase64();
    buf.close();
    return hexed;
}


