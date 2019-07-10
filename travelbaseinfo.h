#ifndef TRAVELBASEINFO_H
#define TRAVELBASEINFO_H
#include <QString>
#include <QWidget>
class travelBase {
public:
    void setEmty();
    int gettravelNumber (){return this->travelNumber;}
    void settravelNumber(const int travelNumber){this->travelNumber= travelNumber;}
    QString gettraveldate(){return this->traveldate;}
    void settraveldate(const QString traveldate){this->traveldate = traveldate;}
    QString gettravelPeople(){
       return this->travelPeople;
    }
    void settravelPeople(const QString travelPeople){
        this->travelPeople =travelPeople;
    }
    QString gettravelPdpart(){
        return this->travelPdpart;
    }
    void settravelPdpart(const QString travelPdpart){
        this->travelPdpart= travelPdpart;
    }
    QString gettravelUse(){
        return this->travelUse;
    }
    void settravelUse(const QString travelUse){
        this->travelUse = travelUse;
    }
//    QString getisSchool(){
//        return this->isSchool;
//    }
//    void setisSchool(const QString isSchool){
//        this->isSchool =isSchool;
//    }
//    QString getstaffName(){
//        return this->staffName;
//    }
//    void setstaffName(const QString staffName){
//        this->staffName =staffName;
//    }
//    int setstaffNumber(){
//        return this->staffNumber;
//    }
//    void setstaffNumber(const int staffNumber){
//        this->staffNumber =staffNumber;
//    }
//    QString getleaveDate(){
//        return this->leaveDate;
//    }
//    void setleaveDate(const QString leaveDate){
//        this->leaveDate =leaveDate;
//    }
//    QString getreturnDate(){
//       return this->returnDate;
//    }
//    void setreturnDate(const QString returnDate){
//        this->returnDate=returnDate;
//    }
//    QString getdays(){
//        return this->days;
//    }
//    void setdays(const QString days){
//        this->days =days;
//    }
//    QString getdepartment(){
//        return this->department;
//    }
//    void setdepartment(const QString department){
//        this->department =department;
//    }
//    QString getleavePlace(){
//        return this->leavePlace;
//    }
//    void setleavePlace(const QString leavePlace){
//        this->leavePlace =leavePlace;
//    }
//    QString getarrivePlace(){
//        return this->arrivePlace;
//    }
//    void setarrivePlace(const QString arrivePlace){
//        this->arrivePlace=arrivePlace;
//    }
//    int getbudgetNum(){
//        return this->budgetNum;
//    }
//    void setbudgetNum(const int budgetNum){
//        this->budgetNum =budgetNum;
//    }
//    QString getbudgetName(){
//        return this->budgetName;
//    }
//    void setbudgetName(const QString budgetName){
//        this->budgetName =budgetName;
//    }
//    QString getbudgetType(){
//        return this->budgetType;
//    }
//    void setbudgetType(const QString budgetType){
//        this->budgetType=budgetType;
//    }
//    float getcityFee(){
//        return this->cityFee;
//    }
//    void setcityFee(const float cityFee){
//        this->cityFee = cityFee;
//    }
//    float getstayFee(){
//        return this->stayFee;
//    }
//    void setstayFee(const float stayFee){
//        this->stayFee = stayFee;
//    }
//    float getworkFee(){
//        return this->workFee;
//    }
//    void setworkFee(const float workFee){
//        this->workFee =workFee;
//    }
//    float getlunchDays(){
//        return this->lunchDays;
//    }
//    void setlunchDays(const float lunchDays){
//        this->lunchDays=lunchDays;
//    }
//    float getlunchSubsidy(){
//        return this->lunchSubsidy;
//    }
//    void setlunchSubsidy(const float lunchSubsidy){
//        this->lunchSubsidy=lunchSubsidy;
//    }
//    float  getairportFee(){
//        return this->airportFee;
//    }
//    void setairportFee(const float airportFee){
//        this->airportFee=airportFee;
//    }
//    float gettraDays(){
//        return this->traDays;
//    }
//    void settraDays(const float traDays){
//        this->traDays=traDays;
//    }
//    float gettraSubsidy(){
//        return this->traSubsidy;
//    }
//    void settraSubsidy(const float traSubsidy){
//        this->traSubsidy =traSubsidy ;
//    }
//    float getotherFee(){
//        return this->otherFee;
//    }
//    void setotherFee(const float otherFee){
//        this->otherFee=otherFee;
//    }



private:
    int travelNumber;//单据编号
    QString traveldate ;//'单据日期',
    QString travelPeople;// '经办人',
    QString travelPdpart;//'经办人部门',
    QString travelUse;//出差事由',
//    QString isSchool;// '是否院内人员',
//    QString staffName;// '出差人员名',
//    int staffNumber;//员工工号',
//    QString leaveDate;//'出发时间',
//    QString returnDate;//'返回时间',
//    QString days;//'出差天数',
//    QString department;// '部门',
//    QString leavePlace;// '出发地点',
//    QString arrivePlace;// '到达地点',
//    int budgetNum;//'预算项目号/经费号',
//    QString budgetName;//'预算项目名称',
//    QString budgetType;//'经费类型',
//    float  cityFee;// '城市间交通费',
//    float  stayFee;// '住宿费',
//    float  workFee;// '会务费/注册费',
//    float  lunchDays;//'伙食补贴天数',
//    float  lunchSubsidy;// '伙食补贴费',
//    float airportFee;//'往返机场交通费',
//    float  traDays;//'交通补贴天数',
//    float  traSubsidy;// '交通补贴费',
//    float otherFee;//'其他费用'
};

class travelBaseInfo : public QWidget
{
    Q_OBJECT
public:
    explicit travelBaseInfo(QWidget *parent = 0);

signals:

public slots:
};

#endif // TRAVELBASEINFO_H
