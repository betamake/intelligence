#ifndef BILLCHECK_H
#define BILLCHECK_H
#include<iostream>
using namespace std;
#include<QString>
#include <QObject>
#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif
class BillCheck
{

public:

    BillCheck();
    ~BillCheck();

    void setEmpty();

    QString getBillid();
    void setBillid(const QString billID);

    QString getBillexpenseid();
    void setBillexpenseid(const QString billExpenseid);

    QString getBusid();
    void setBusid(const QString busid);

    QString getCostid();
    void setCostid(const QString costid);

    QString getBillattachmenttitle();
    void setBillattachmenttitle(const QString billAttachmenttitle);

    QString getBillrealpath();
    void setBillrealpath(const QString billRealpath);

    QString getBilltype();
    void setBilltype(const QString billType);

    QString getBillcontent();
    void setBillcontent(const QString billContent);

    QString getBillcode();
    void setBillcode(const QString billCode);

    QString getBillnumber();
    void setBillnumber(const QString billNumber);

    QString getBilluser();
    void setBilluser(const QString billUser);

    QString getBillstartplace();
    void setBillstartplace(const QString billStartplace);

    QString getBillendplace();
    void setBillendplace(const QString billEndplace);

    QString getBilldate();
    void setBilldate(const QString billDate);

    QString getBillmoney();
    void setBillmoney(const QString billMoney);

    QString getBillcheckcode();
    void setBillcheckcode(const QString billCheckcode);

    QString getBillother();
    void setBillother(const QString billOther);

    QString toString();

private:
    QString billID;//报销人id
    QString billExpenseid;
    QString busid;//差旅报销单号
    QString costid;//费用报销单号
    QString billAttachmenttitle;//附件名称
    QString billRealpath;//文件保存路径
    QString billType;//票据类型
    QString billContent;//票据名目
    QString billCode;//票据代码
    QString billNumber;//票据号码
    QString billUser;//火车票、飞机票乘车人
    QString billStartplace;//始发站
    QString billEndplace;//终点站
    QString billDate;//发票日期、乘车日期
    QString billMoney;//票据金额
    QString billCheckcode;//校验码
    QString billOther;//发票的号码
};

#endif // BILLCHECK_H
