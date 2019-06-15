#include "billcheck.h"
#include <QDebug>

BillCheck::BillCheck()
{
    /*
        QString billID;//报销人id
        QString billExpenseid;
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
        QString billOther;//发票的号码*/
}
BillCheck::~BillCheck()
{
    setBillattachmenttitle ("");
    setBillcheckcode ("");
    setBillcode ("");
    setBillcontent ("");
    setBilldate ("");
    setBillendplace ("");
    setBillexpenseid ("");
    setBillid ("");
    setBillmoney ("");
    setBillnumber ("");
    setBillother ("");
    setBillrealpath ("");
    setBillstartplace ("");
    setBilltype ("");
    setBilluser ("");
    setBusid ("");
    setCostid ("");
}

void BillCheck::setEmpty ()
{
    setBillattachmenttitle ("");
}
QString BillCheck::getBillid()
{
    return billID;
}
void BillCheck::setBillid(const QString billID)
{
    this->billID = billID;
}

QString BillCheck::getBillexpenseid()
{
    return billExpenseid;
}
void BillCheck::setBillexpenseid(const QString billExpenseid)
{
    this->billExpenseid = billExpenseid;
}

QString BillCheck::getBusid(){
    return busid;
}

void BillCheck::setBusid(const QString busid){
    this->busid = busid;
}

QString BillCheck::getCostid(){
    return costid;
}

void BillCheck::setCostid(const QString costid){
    this->costid = costid;
}

QString BillCheck::getBillattachmenttitle()
{
    return billAttachmenttitle;
}
void BillCheck::setBillattachmenttitle(const QString billAttachmenttitle)
{
    this->billAttachmenttitle = billAttachmenttitle;
}

QString BillCheck::getBillrealpath()
{
    return billRealpath;
}
void BillCheck::setBillrealpath(const QString billRealpath)
{
    this->billRealpath = billRealpath;
}

QString BillCheck::getBilltype()
{
    return billType;
}

void BillCheck::setBilltype(const QString billType)
{
    this->billType = billType;
}
QString BillCheck::getBillcontent()
{
    return billContent;
}
void BillCheck::setBillcontent(const QString billContent)
{
    this->billContent = billContent;
}

QString BillCheck::getBillcode()
{
    return billCode;
}
void BillCheck::setBillcode(const QString billCode)
{
    this->billCode = billCode;
}

QString BillCheck::getBillnumber()
{
    return billNumber;
}
void BillCheck::setBillnumber(const QString billNumber)
{
    this->billNumber = billNumber;
}

QString BillCheck::getBilluser()
{
    return billUser;
}
void BillCheck::setBilluser(const QString billUser)
{
    this->billUser = billUser;
}

QString BillCheck::getBillstartplace()
{
    return billStartplace;
}
void BillCheck::setBillstartplace(const QString billStartplace)
{
    this->billStartplace = billStartplace;
}

QString BillCheck::getBillendplace()
{
    return billEndplace;
}
void BillCheck::setBillendplace(const QString billEndplace)
{
    this->billEndplace = billEndplace;
}

QString BillCheck::getBilldate()
{
    return billDate;
}
void BillCheck::setBilldate(const QString billDate)
{
    this->billDate = billDate;
}

QString BillCheck::getBillmoney()
{
    return billMoney;
}
void BillCheck::setBillmoney(const QString billMoney)
{
    this->billMoney = billMoney;
}

QString BillCheck::getBillcheckcode()
{
    return billCheckcode;
}
void BillCheck::setBillcheckcode(const QString billCheckcode)
{
    this->billCheckcode = billCheckcode;
}

QString BillCheck::getBillother()
{
    return billOther;
}
void BillCheck::setBillother(const QString billOther)
{
    this->billOther = billOther;
}
/*
    QString billID;//报销人id
    QString billExpenseid;
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
    QString billOther;//发票的号码*/

QString BillCheck::toString()
{
    return "[报销人id:"+billID+" ,billExpenseid:"+billExpenseid+
            " ,\n附件名称:"+billAttachmenttitle+" ,文件保存路径:"+billRealpath+
            " ,\n票据类型:"+billType+" ,票据名目:"+billContent+
            " ,\n票据号码:"+billCode+" ,票据号码:"+billNumber+
            " ,\n乘车人:"+billUser+" ,始发站:"+billStartplace+
            " ,\n终点站:"+billEndplace+" ,发票/乘车日期:"+billDate+
            " ,\n票据金额:"+billMoney+" ,校验码:"+billCheckcode+
            " ,发票号码:"+billOther+"]";
}
