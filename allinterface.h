#ifndef ALLINTERFACE_H
#define ALLINTERFACE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QString>
#include <commonutils.h>
#include <QVariant>
#include <QList>
#include <QUrl>
#include <QNetworkCookieJar>
class InterfaceInfo{
public:
    QString getname(){
        return this->name;
    }
    void setname(const QString name){
        this->name = name;
    }
    QString getmsg(){
        return this->msg;
    }
    void setmsg(const QString msg){
        this->msg= msg;
    }
    QString getpassword(){
        return this->password;
    }
    void setpassword(const QString password){
        this->password = password;
    }
    QString getusername(){
        return this->username;
    }
    void setusername(const QString username){

        this->username=username;
    }
    QString getofficeName(){
        return this->officeName;
    }
    void setofficeName(const QString officeName){
        this->officeName = officeName;
    }

private:
    QString name; //用户名
    QString username; //登录名
    QString msg;
    QString password;//用户密码
    QString officeName;//部门名称

};

class allInterface : public QWidget
{
    Q_OBJECT
public:
    explicit allInterface(QWidget *parent = 0);

    InterfaceInfo getinfo(){
        return info;
    }
    InterfaceInfo info;
    void userInterface(); //获取用户登录信息;
//    void getDataPeopleOut();//获取院外人员信息.

signals:
    void readUserDone();

public slots:
    void readUserInterfaceinfo(QNetworkReply *reply); //保存用户信息
    void getuserdatalist(); //获取用户信息
    void setuserdatalist(QNetworkReply *reply); //保
private:
    QNetworkAccessManager *manager;
    QString ipAddress;
    CommonUtils commonutils;
//    QList<QNetworkCookie> allcookies;
//    QVariant varCookie;
    QNetworkCookieJar *managerJar;

};
#endif // ALLINTERFACE_H

