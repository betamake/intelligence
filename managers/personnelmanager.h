#ifndef PERSONNELMANAGER_H
#define PERSONNELMANAGER_H

#include <QObject>
#include <QDate>

//出国人员信息，经费来源和报销明细
class abroadPersonInfo {
public:
    abroadPersonInfo();

    void setEmpty();

    QString getStaffName() {return this->staffName;}
    void setStaffName(const QString staffName) { this->staffName = staffName;}

    int getStaffNumber() {return this->staffNumber;}
    void setStaffNumber(const int staffNumber) { this->staffNumber = staffNumber;}

//    QString getPayInfoBankNum() ;
//    void setPayInfoBankNum(const QString payInfoBankNum);

    QString getDepartment() { return this->department;}
    void setDepartment(const QString department) { this->department = department;}

    QString getLeaveDate() {return this->leaveDate;}
    void setLeaveDate(const QString leaveDate) { this->leaveDate = leaveDate;}

    QDate getLeaveDate1() { return this->leaveDate1;}
    void setLeaveDate1(const QDate leaveDate) { this->leaveDate1 = leaveDate;}

    int getDays() { return this->days;}
    void setDays(int days) { this->days = days;}

    QString getReturnDate() { return this->returnDate;}
    void setReturnDate(const QString returnDate) { this->returnDate = returnDate;}

    QDate getReturnDate1() { return this->returnDate1;}
    void setReturnDate1(const QDate returnDate) { this->returnDate1 = returnDate;}

//    QString getCountry() { return this->country;}
//    void setCountry(const QString country) { this->country = country;}

    QString getLeaveCity() { return this->leaveCity;}
    void setLeaveCity(const QString leaveCity) { this->leaveCity = leaveCity;}

    QString getArriveCity() { return this->arriveCity;}
    void setArriveCity(const QString arriveCity) { this->arriveCity = arriveCity;}

    QString getCertificateType() { return this->certificateType;}
    void setCertificateType(const QString certificateType) { this->certificateType = certificateType;}

    int getBudgetNum() { return this->budgetNum;}
    void setBudgetNum(const int budgetNum) { this->budgetNum = budgetNum;}

    QString getBudgetName() { return this->budgetName;}
    void setBudgetName(const QString budgetName) { this->budgetName = budgetName;}

    QString getBudgetType() { return this->budgetType;}
    void setBudgetType(const QString budgetType) { this->budgetType = budgetType;}

    int getTotalFee() { return this->totalFee;}
    void setTotalFee(const int totalFee) { this->totalFee = totalFee;}

    QString toString();

private:
    QString staffName;          //员工姓名
    int     staffNumber;        //员工工号
    QString department;         //部门
    QString leaveDate;          //出访起始时间
    QDate   leaveDate1;
    QString returnDate;         //出访终止时间
    QDate   returnDate1;
    int     days;               //天数
//    QString country;            //出访国家
    QString leaveCity;          //出发城市
    QString arriveCity;         //到达城市
    QString certificateType;    //证件类型

    int     budgetNum;          //预算项目号/经费号
    QString budgetName;         //预算项目名称
    QString budgetType;         //经费类型

    int     totalFee;           //总费用，先不管8个分支的费用计算
};

//出差人员信息，经费来源和报销明细
class traBusPersonInfo {
public:
    traBusPersonInfo();

    void setEmpty();

    bool getIsInSchool() { return this->isInSchool;}
    void setIsInSchool(bool isInSchool) { this->isInSchool = isInSchool;}

    QString getStaffName() {return this->staffName;}
    void setStaffName(const QString staffName) { this->staffName = staffName;}

    int getStaffNumber() { return this->staffNumber;}
    void setStaffNumber(const int staffNumber) { this->staffNumber = staffNumber;}

    QString getLeaveDate() { return this->leaveDate;}
    void setLeaveDate(const QString leaveDate) { this->leaveDate = leaveDate;}

    QDate getLeaveDate1() { return this->leaveDate1;}
    void setLeaveDate1(const QDate leaveDate) { this->leaveDate1 = leaveDate;}

    QString getReturnDate() { return this->returnDate;}
    void setReturnDate(const QString returnDate) { this->returnDate = returnDate;}

    QDate getReturnDate1() { return this->returnDate1;}
    void setReturnDate1(const QDate returnDate) { this->returnDate1 = returnDate;}

    int getDays() { return this->days;}
    void setDays(const int days) { this->days = days;}

    QString getDepartment() { return this->department;}
    void setDepartment(const QString department) { this->department = department;}

    QString getLeavePlace() { return this->leavePlace;}
    void setLeavePlace(const QString leavePlace) { this->leavePlace = leavePlace;}

    QString getArrivePlace() { return this->arrivePlace;}
    void setArrivePlace(const QString arrivePlace) { this->arrivePlace = arrivePlace;}

    int getBudgetNumber() { return this->budgetNum;}
    void setBudgetNumber(const int budgetNum) { this->budgetNum = budgetNum;}

    QString getBudgetName() { return this->budgetName;}
    void setBudgetName(const QString budgetName) { this->budgetName = budgetName;}

    QString getBudgetType() { return this->budgetType;}
    void setBudgetType(const QString budgetType) { this->budgetType = budgetType;}

    int getLunchPerDay() { return this->lunchPerDay;}
    void setLunchPerDay(int per) { this->lunchPerDay = per;}

    int getTravelPerDay() { return this->travelPerDay;}
    void setTravelPerDay(int per) { this->travelPerDay = per;}

    int getStayPerDay() { return this->stayPerDay;}
    void setStayPerDay(int per) { this->stayPerDay = per;}

    int getCityFee() { return this->cityFee;}
    void setCityFee(const int cityFee) { this->cityFee = cityFee;}

    int getStayFee() { return this->stayFee;}
    void setStayFee(const int stayFee) { this->stayFee = stayFee;}

    int getWorkFee() { return this->workFee;}
    void setWorkFee(const int workFee) { this->workFee = workFee;}

    int getLunchDays() { return this->lunchDays;}
    void setLunchDays(const int lunchDays) { this->lunchDays = lunchDays;}

    int getLunchSubsidy() { return this->lunchSubsidy;}
    void setLunchSubsidy(const int lunchSubsidy) { this->lunchSubsidy = lunchSubsidy;}

    int getAirportFee() { return this->airportFee;}
    void setAirportFee(const int airportFee) { this->airportFee = airportFee;}

    int getTraDays() { return this->traDays;}
    void setTraDays(const int traDays) { this->traDays = traDays;}

    int getTraSubsidy() { return this->traSubsidy;}
    void setTraSubsidy(const int traSubsidy) { this->traSubsidy = traSubsidy;}

    int getOtherFee() { return this->otherFee;}
    void setOtherFee(const int otherFee) { this->otherFee = otherFee;}

    int getTotalFee() { return this->totalFee;}
    void setTotalFee(const int totalFee) { this->totalFee = totalFee;}

    QString toString();

private:
    bool    isInSchool;         //true为院内人员，其他不是院内人员
    QString staffName;          //出差人员名
    int     staffNumber;        //员工工号
    QString leaveDate;          //出发时间
    QDate   leaveDate1;
    QString returnDate;         //返回时间
    QDate   returnDate1;
    int     days;               //出差天数
    QString department;         //部门
    QString leavePlace;         //出发地点
    QString arrivePlace;        //到达地点

    int     budgetNum;          //预算项目号/经费号
    QString budgetName;         //预算项目名称
    QString budgetType;         //经费类型

    int     lunchPerDay;        //伙食每日补贴
    int     travelPerDay;       //交通每日补贴
    int     stayPerDay;         //住宿每日补贴

    int     cityFee;            //城市间交通费
    int     stayFee;            //住宿费
    int     workFee;            //会务费/注册费
    int     lunchDays;          //伙食补贴天数
    int     lunchSubsidy;       //伙食补贴费
    int     airportFee;         //往返机场交通费
    int     traDays;            //交通补贴天数
    int     traSubsidy;         //交通补贴费
    int     otherFee;           //其他费用

    int     totalFee;           //总费用
};


class personnelManager
{
public:
    static personnelManager *getInstance();
    personnelManager();
    ~personnelManager();

    void addAbroadItem(abroadPersonInfo *info) { abroadList.append(info);}
//    void delAbroadItem(abroadPersonInfo *info);
    QList<abroadPersonInfo*> getAbroadList() { return abroadList;}

    void addTravelItem(traBusPersonInfo *info) { travelList.append(info);}
//    void delTravelItem(traBusPersonInfo *info);
    QList<traBusPersonInfo*> getTravelList() { return travelList;}

private:
    static personnelManager *instance;

    QList<abroadPersonInfo*> abroadList;
    QList<traBusPersonInfo*> travelList;
};

#endif // PERSONNELMANAGER_H
