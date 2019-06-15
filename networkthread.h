//#ifndef NETWORKTHREAD_H
//#define NETWORKTHREAD_H
//using namespace std;

//#include <QObject>
//#include <QMutex>
//#include <QNetworkAccessManager>
//#include "commonutils.h"
//#include "databaseutils.h"
//#include "user.h"
//#include <QMap>
//#include <QNetworkReply>
//class NetworkThread : public QObject
//{
//    Q_OBJECT
//public:
//    //    explicit NetworkThread(QObject *parent = 0);
//    NetworkThread(QObject *parent = NULL);

//    ~NetworkThread();
//    void setRunCount(int count);
//    void setData(QByteArray data);

//    void stop();
//    //    QByteArray getPixmapData(QString filePath,QImage image);

//signals:
//    void message(const QString& info);
//    void progress(int present);
//    void emitCode(int , QMap<QString, QString>);

//public slots:
//    void runfaceCheckNetwork();
//    void faceCheckReply(QNetworkReply*);
//    void runNetwork();
//    void runNetwork2();
//    void deletePhoto(QString uid);
//    void faceDelReply(QNetworkReply *reply);
//    void receiveByteData(QByteArray data);
//private:
//    bool m_isStop;
//    int m_runCount;
//    int m_runCount2;
//    QMutex m_stopMutex;
//    QNetworkAccessManager *manager;
//    QByteArray received_data;
//    CommonUtils commonutils;
//    databaseUtils database;
//    QString address;
//    QMap<QString, QString>  map;

//};

//#endif // NETWORKTHREAD_H
