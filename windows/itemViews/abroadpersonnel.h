#ifndef ABROADPERSONNEL_H
#define ABROADPERSONNEL_H

#include <QWidget>

#include "windows/itemViews/abroadfeeitem.h"

namespace Ui {
class abroadPersonnel;
}

//出差人员信息，经费来源和报销明细
class abroadPersonInfo {
public:
    abroadPersonInfo();

    void setEmpty();

    QString getStaffName() {return this->staffName;}
    void setStaffName(const QString staffName) { this->staffName = staffName;}

    int getStaffNumber();
    void setStaffNumber(const int staffNumber) { this->staffNumber = staffNumber;}

    QString getPayInfoBankNum();
    void setPayInfoBankNum(const QString payInfoBankNum);

    QString getDepartment() { return this->department;}
    void setDepartment(const QString department) { this->department = department;}

    QString getLeaveDate() {return this->leaveDate;}
    void setLeaveDate(const QString leaveDate) { this->leaveDate = leaveDate;}

    QString getReturnDate() { return this->returnDate;}
    void setReturnDate(const QString returnDate) { this->returnDate = returnDate;}

    QString getCountry() { return this->country;}
    void setCountry(const QString country) { this->country = country;}

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
    QString returnDate;         //出访终止时间
    QString country;            //出访国家
    QString leaveCity;          //出发城市
    QString arriveCity;         //到达城市
    QString certificateType;    //证件类型

    int     budgetNum;          //预算项目号/经费号
    QString budgetName;         //预算项目名称
    QString budgetType;         //经费类型

    int     totalFee;           //总费用，先不管8个分支的费用计算
};

class abroadPersonnel : public QWidget
{
    Q_OBJECT

public:
    explicit abroadPersonnel(QWidget *parent = nullptr);
    ~abroadPersonnel();

    void setIndex(int index);

private:
    void addListItems();

private:
    Ui::abroadPersonnel *ui;
};

#endif // ABROADPERSONNEL_H
