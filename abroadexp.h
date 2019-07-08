#ifndef ABROADEXP_H
#define ABROADEXP_H
#include"iostream"
#include"QString"
#include "expensehead.h"
#include "busiexp.h"
using namespace std;

class abroadExp
{
private:
    int id;//id
    QString abdid;//出国报销单id
    QString abdManid;//出国人id
    QString abdVisacost;//签证费金额
    QString abdVisabillnum;//签证费单据张数
    QString abdServcost;//服务费金额
    QString abdServbillnum;//服务费票据张数
    QString abdExchgcost;//出国换汇金额
    QString abdExchgbillnum;//出国换汇金额票据张数

    //ExpenseHead abroadHead;//表头对象
    //busiExp abroadBusi;
    //deshi


public:
    abroadExp();
    ~abroadExp();

    int getid();
    void setid(const int id);

    QString getAbdid();
    void setAbdid(const QString abdid);

    QString getAbdmanid();
    void setAbdmanid(const QString abdManid);

    QString getAbdvisacost();
    void setAbdvisacost(const QString abdVisabillnum);

    QString getAbdvisabillnum();
    void setAbdvisabillnum(const QString abdVisabillnum);

    QString getAbdservcost();
    void setAbdservcost(const QString abdServcost);

    QString getAbdservbillnum();
    void setAbdservbillnum(const QString abdServbillnum);

    QString getAbdexchgcost();
    void setAbdexchgcost(const QString abdExchgcost);

    QString getAbdexchgbillnum();
    void setAbdexchgbillnum(const QString abdExchgbillnum);

    void toString();

};

#endif // ABROADEXP_H
