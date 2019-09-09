#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkRequest>


class CommonUtils : public QObject
{
    Q_OBJECT
public:
    explicit CommonUtils(QObject *parent = 0);
    QString getCurrentTime();
    QByteArray Image_To_Base64(QString ImgPath);
//    void printFile(QString dataFileName,QByteArray fdata);
    QNetworkRequest getHttpRequest(QString url);
    QNetworkRequest getHttpRequestRemote(QString url);

signals:

public slots:
};

#endif // COMMONUTILS_H
