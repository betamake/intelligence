#include "payinfo.h"

PayInfo::PayInfo()
{
//    QString payinfoType;//支付方式 待定
//    QString payInfoUnit;//收款人
//    QString payInfoBank;//所属银行
//    QString payInfoBankName;//开户行名称
//    QString payInfoBankNum;//银行卡号
//    QString payInfoBill;//金额
//    QString payInfoNote;//备注
}

void PayInfo::setEmpty ()
{
    setPayInfoBank ("");
    setPayInfoBankName ("");
    setPayInfoBankNum ("");
    setPayInfoBill ("");
    setPayInfoBusid ("");
    setPayInfoCostid ("");
    setPayInfoNote ("");
    setPayinfoType ("");
    setPayInfoUnit ("");
}

QString PayInfo::getPayInfoBusid(){
    return payBusid;
}

void PayInfo::setPayInfoBusid(const QString payBusid){
    this->payBusid = payBusid;
}

QString PayInfo::getPayInfoCostid(){
    return payCostid;
}

void PayInfo::setPayInfoCostid(const QString payCostid){
    this->payCostid = payCostid;
}

QString PayInfo::getPayinfoType()
{
    return payinfoType;
}
void PayInfo::setPayinfoType(const QString payinfoType)
{
    this->payinfoType = payinfoType;
}

QString PayInfo::getPayInfoUnit()
{
    return payInfoUnit;
}
void PayInfo::setPayInfoUnit(const QString payInfoUnit)
{
    this->payInfoUnit = payInfoUnit;
}


QString PayInfo::getPayInfoBank()
{
    return payInfoBank;
}
void PayInfo::setPayInfoBank(const QString payInfoBank)
{
    this->payInfoBank = payInfoBank;
}


QString PayInfo::getPayInfoBankName()
{
    return payInfoBankName;
}
void PayInfo::setPayInfoBankName(const QString payInfoBankName)
{
    this->payInfoBankName = payInfoBankName;
}

QString PayInfo::getPayInfoBankNum()
{
    return payInfoBankNum;
}
void PayInfo::setPayInfoBankNum(const QString payInfoBankNum)
{
    this->payInfoBankNum = payInfoBankNum;
}

void PayInfo::setPayInfoBill(const QString payInfoBill)
{
    this->payInfoBill = payInfoBill;
}
QString PayInfo::getPayInfoBill()
{
    return payInfoBill;
}

void PayInfo::setPayInfoNote(const QString payInfoNote)
{
    this->payInfoNote = payInfoNote;
}
QString PayInfo::getPayInfoNote()
{
    return payInfoNote;
}
QString PayInfo::toString()
{
    //    QString payinfoType;//支付方式 待定
    //    QString payInfoUnit;//收款人
    //    QString payInfoBank;//所属银行
    //    QString payInfoBankName;//开户行名称
    //    QString payInfoBankNum;//银行卡号
    //    QString payInfoBill;//金额
    //    QString payInfoNote;//备注
    return "[收款人:"+payInfoUnit+
           ",开户行名称:"+payInfoBankName+
            ",\n支付方式:"+payinfoType+
            ",所属银行:"+payInfoBank+
            ",\n开户行名称:"+payInfoBankNum+
            ",金额:"+payInfoBill
            +",\n备注:"+payInfoNote+
            "]";
}
