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
    QString getsessionid(){
        return this->sessionid;
    }
    void setsessionid(const QString sessionid){
        this->sessionid = sessionid;

    }
    QString getpeopleOutName(){
        return this->peopleOutname;
    }
    void setpropleOutName(const QString peopleOutname)
    {
        this->peopleOutname = peopleOutname;
    }
    QString getCode(){
       return this->code;
    }
    void setCode(const QString code){
        this->code=code;
    }
    QString getRandId(){
        return this->rankId;
    }
    void setRandId(const QString rankId){
        this->rankId = rankId;
    }
    QString getarriveCity()
    {
        return this->arriveCity;
    }
    void setarriveCity(const QString arriveCity)
    {
        this->arriveCity =arriveCity;
    }
    int getstaySubsidy()
    {
        return this->staySubsidy;
    }
    void setstaySubsidy(const  int staySubsidy){
        this->staySubsidy =staySubsidy;
    }
     int  gettrafficSubsidy(){
        return this->trafficSubsidy;
    }
    void settrafficSubsidy(const  int trafficSubsidy){
        this->trafficSubsidy =trafficSubsidy;
    }
     int getmealSubsidy(){
        return this->mealSubsidy;
    }
    void setmealSubsidy(const  int mealSubsidy){
        this->mealSubsidy=mealSubsidy;
    }
    QString getrankName(){
        return this->rankName;
    }
    void setrankName(const QString rankName){
        this->rankName=rankName;
    }
    int getcommonMiscellaneous(){
        return this->commonMiscellaneous;
    }
    void setcommonMiscellaneous(const int commonMiscellaneous){
        this->commonMiscellaneous=commonMiscellaneous;
    }
    int getaboradStaySubsidy()
    {
        return this->aboradStaySubsidy;
    }
    void setaboradStaySubsidy(const int aboradStaySubsidy)
    {
        this->aboradStaySubsidy=aboradStaySubsidy;
    }
    int getaboradMealSubsidy(){
        return this->aboradMealSubsidy;
    }
    void setaboradMealSubsidy(const int aboradMealSubsidy){
        this->aboradMealSubsidy=aboradMealSubsidy;
    }
    QString getcurrencyName(){
        return this->currencyName;
    }
    void setcurrencyName(const QString currencyName){
        this->currencyName=currencyName;
    }

private:
    //用户登录信息
    QString name; //用户名
    QString username; //登录名
    QString msg;
    QString password;//用户密码
    //用户个人信息
    QString code;
    QString officeName;//部门名称
    QString province; //省份
    QString city; //城市
    QString bankName; //银行
    QString bankAccount;//银行卡号
    QString rankId;//用户级别Id
    QString rankName;//用户级别名称
    //经费信息
    QString fundVsersion; //经费号
    QString projectName; //项目名称
    QString moneyOriginName; //经费类型;
    //业务事项
    QString reimburseName; //业务事项名字
    //cookie
    QString sessionid;
    // 院外人员信息
    QString peopleOutname; //院外人员信息名字
    //国内差旅标准
    QString arriveCity; //到达城市
    int staySubsidy; //住宿标准
    int trafficSubsidy;//交通费标准
    int mealSubsidy; //伙食标准;
    //国际差旅标准
    int aboradStaySubsidy; //国外注册费用
    int commonMiscellaneous;//公杂费
    int aboradMealSubsidy;//国外伙食标准
    QString currencyName;//币种



};

class allInterface : public QWidget
{
    Q_OBJECT
public:
    explicit allInterface(QWidget *parent = 0);
    ~allInterface();
    InterfaceInfo getinfo(){
        return info;
    }
    InterfaceInfo info;
    void userInterface(); //获取用户登录信息;
    void getDataPeopleOut();//获取院外人员信息.
    void getDataProject();//获取经费类型
    void getdataTravel();//获取国内差旅标准信息;
    void getdataCountriesExpense();//获取国外信差旅标准;

    static allInterface *getinstance();
signals:
    void readUserDone();
    void setDataProjectDone();
    void setUserDataListDone();
    void setdataCountriesExpenseDone();
    void setdataTravelDone ();

public slots:
    void readUserInterfaceinfo(QNetworkReply *reply); //保存用户登录信息
    void getuserdatalist(); //获取用户信息
    void setuserdatalist(QNetworkReply *reply); //保存用户信息
    void setDatapeopleOut(QNetworkReply *reply);//保存院外人员信息
    void setDataProject(QNetworkReply *reply);//保存经费类型
    void setdataTravel(QNetworkReply *reply);//保存国内差旅标准信息;
    void setdataCountriesExpense(QNetworkReply *reply);// 保存国外差旅标准信息;

private:

    QString ipAddress;
    CommonUtils commonutils;
//    QList<QNetworkCookie> allcookies;
//    QVariant varCookie;
//    QString url;
    QNetworkCookieJar *managerJar;
    static allInterface *instance;

};
#endif // ALLINTERFACE_H

