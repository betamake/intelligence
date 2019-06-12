//#ifndef NETWORKHANDLER_H
//#define NETWORKHANDLER_H

//#include <QObject>
//#include <QTimer>
//#include<QCamera>
//#include<QCameraViewfinder>
//#include<QCameraImageCapture>
//#include<QNetworkAccessManager>
//#include <QMap>
//#include <QNetworkReply>
//#include "commonutils.h"
//#include "databaseutils.h"
//#include "user.h"


//class NetworkHandler : public QObject
//{
//    Q_OBJECT
//public:
//    NetworkHandler(QObject *parent = NULL);
//    ~NetworkHandler();
//    QByteArray getPixmapData(QString filePath,QImage image);

//signals:
////    void dataRecved();
//    void updateCarmer(int idFace);
//    void emitCode(int , QMap<QString, QString>);


//public slots:
//    void faceCheck();
//    void sendPhoto(int id,QImage image);
//    void faceDelReply(QNetworkReply *reply);
//    void deletePhoto(QString uid);
//    void faceCheckReply(QNetworkReply *reply);

//private:
//    QTimer *m_RecvTask;
//    int facetime;
//    QNetworkAccessManager * manager;
//    CommonUtils commonutils;
//    databaseUtils database;
//    QString address;
//    QMap<QString, QString> map;
//};

//#endif // NETWORKHANDLER_H
