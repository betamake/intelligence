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
    QString getprovice(){
        return this->province;
    }
    void setprovince(const QString province){
        this->province =province;
    }
    QString getcity(){
        return this->city;
    }
    void setcity(const QString city){
        this->city =city;
    }
    QString getbankName(){
        return this->bankName;
    }
    void setbankName(const QString bankName){
        this->bankName = bankName;
    }
    QString getbankAccount(){
        return this->bankAccount;
    }
    void setbankAccount(const QString bankAccount)
    {
        this->bankAccount = bankAccount;
    }
    QString getfundVersion(){
        return this->fundVsersion;
    }
    void setfundVersion(const QString fundVsersion){
        this->fundVsersion = fundVsersion;
    }
    QString getprojectName(){
        return this->projectName;
    }
    void setprojectName(const QString projectName)
   {
        this->projectName = projectName;
    }
    QString getmoneyOriginName(){
        return this->moneyOriginName;
    }
    void setmoneyOriginName(const QString moneyOriginName ){
        this->moneyOriginName = moneyOriginName;
    }
    QString getreimburseName(){
        return this->reimburseName;
    }
    void setreimburseName(const QString reimburseName)
    {
        this->reimburseName =reimburseName;
    }


private:
    //用户登录信息
    QString name; //用户名
    QString username; //登录名
    QString msg;
    QString password;//用户密码
    //用户个人信息
    QString officeName;//部门名称
    QString province; //省份
    QString city; //城市
    QString bankName; //银行
    QString bankAccount;//银行卡号
    //经费信息
    QString fundVsersion; //经费号
    QString projectName; //项目名称
    QString moneyOriginName; //经费类型;
    //业务事项
    QString reimburseName; //业务事项名字



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
    void getDataProject();//获取经费类型
    void getDataReimberse();//获取业务事项.

signals:
    void readUserDone();
    void setDataProjectDone();

public slots:
    void readUserInterfaceinfo(QNetworkReply *reply); //保存用户登录信息
    void getuserdatalist(); //获取用户信息
    void setuserdatalist(QNetworkReply *reply); //保存用户信息
//    void setDatapeopleOut(QNetworkReply *reply);//保存院外人员信息
    void setDataProject(QNetworkReply *reply);//保存经费类型
    void setDataReimberse(QNetworkReply *reply);//保存
private:
    QNetworkAccessManager *manager;
    QString ipAddress;
    CommonUtils commonutils;
//    QList<QNetworkCookie> allcookies;
//    QVariant varCookie;
    QNetworkCookieJar *managerJar;

};
#endif // ALLINTERFACE_H

