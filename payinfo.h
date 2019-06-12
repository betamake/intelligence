#ifndef PAYINFO_H
#define PAYINFO_H
#include <QDebug>
#include<iostream>
#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif
using namespace std;

class PayInfo
{
public:
    PayInfo();

    void setEmpty();

    QString getPayInfoBusid();
    void setPayInfoBusid(const QString payBusid);

    QString getPayInfoCostid();
    void setPayInfoCostid(const QString payCostid);

    QString getPayInfoBankNum();
    void setPayInfoBankNum(const QString payInfoBankNum);

    QString getPayInfoBankName();
    void setPayInfoBankName(const QString payInfoBankName);

    QString getPayInfoBank();
    void setPayInfoBank(const QString payInfoBank);

    QString getPayInfoBill();
    void setPayInfoBill(const QString payInfoBill);

    QString getPayInfoNote();
    void setPayInfoNote(const QString payInfoNote);

    QString getPayInfoUnit();
    void setPayInfoUnit(const QString payInfoUnit);

    QString getPayinfoType();
    void setPayinfoType(const QString payinfoType);

    QString toString();

private:
    QString payBusid;//差旅支付方式
    QString payCostid;//费用支付方式
    QString payinfoType;//支付方式 待定
    QString payInfoUnit;//收款人
    QString payInfoBank;//所属银行
    QString payInfoBankName;//开户行名称
    QString payInfoBankNum;//银行卡号
    QString payInfoBill;//金额
    QString payInfoNote;//备注
};

#endif // PAYINFO_H
