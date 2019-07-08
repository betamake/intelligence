#ifndef ADDPERSONNEL_H
#define ADDPERSONNEL_H

#include <QWidget>

namespace Ui {
class addPersonnel;
}

//出差人员信息，经费来源和报销明细
class traBusPersonInfo {
public:
    traBusPersonInfo();

    void setEmpty();

    QString getStaffName() {return this->staffName;}
    void setStaffName(const QString staffName) { this->staffName = staffName;}

    int getStaffNumber() { return this->staffNumber;}
    void setStaffNumber(const int staffNumber) { this->staffNumber = staffNumber;};

    QString getLeaveDate() { return this->leaveDate;}
    void setLeaveDate(const QString leaveDate) { this->leaveDate = leaveDate;}

    QString getReturnDate() { return this->returnDate;}
    void setReturnDate(const QString returnDate) { this->returnDate = returnDate;}

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

    QString toString();

private:
    QString staffName;          //出差人员名
    int     staffNumber;        //员工工号
    QString leaveDate;          //出发时间
    QString returnDate;         //返回时间
    int     days;               //出差天数
    QString department;         //部门
    QString leavePlace;         //出发地点
    QString arrivePlace;        //到达地点

    int     budgetNum;          //预算项目号/经费号
    QString budgetName;         //预算项目名称
    QString budgetType;         //经费类型
    int     cityFee;            //城市间交通费
    int     stayFee;            //住宿费
    int     workFee;            //会务费/注册费
    int     lunchDays;          //伙食补贴天数
    int     lunchSubsidy;       //伙食补贴费
    int     airportFee;         //往返机场交通费
    int     traDays;            //交通补贴天数
    int     traSubsidy;         //交通补贴费
    int     otherFee;           //其他费用
};


class addPersonnel : public QWidget
{
    Q_OBJECT

public:
    explicit addPersonnel(QWidget *parent = nullptr);
    ~addPersonnel();

    void setIndex(int index);
    void saveItem();

private:
    void setNemberOnly();   //设置输入框只能输入数字

private slots:
    void on_cleraBtn_clicked();

private:
    Ui::addPersonnel *ui;
};

#endif // ADDPERSONNEL_H
