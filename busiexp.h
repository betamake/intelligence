#ifndef BUSIEXP_H
#define BUSIEXP_H
#include"iostream"
#include"QString"
#include "expensehead.h"
using namespace std;

class busiExp
{
private:
    QString busiManid;//出差人
    QString busid;//差旅报销单号
    QString abdid;//出国单号
    QString busiBegindate;//起始时间
    QString busiEnddate;//终止时间
    QString busiDay;//天数
    QString busiRelaxday;//公休天数
    QString busiFromadres;//出发地点
    QString busiToadres;//到达地点
    QString busiTranscost;//城市间交通费
    QString busiTransbillnum;//交通费单据张数
    QString busiAccomcost;//住宿费用
    QString busiAccombillnum;//住宿单据张数
    QString busiTranssubsidy;//交通补贴
    QString busiFoodsubsidy;//伙食补贴
    QString busiTotalcost;//小计
    QString busiOthercost;//其他费用金额
    QString busiOthernum;//其他费用单据张数
    QString busiOthermemo;//其他费用说明
   // ExpenseHead busi;

public:
    busiExp();
    ~busiExp();

    void setEmpty();
//    void init();

    QString getBusimanid();
    void setBusimanid(const QString busiManid);

    QString getbusid();
    void setbusid(const QString busid);

    QString getBusibegindate();
    void setBusibegindate(const QString busiBegindate);

    QString getAbdid();
    void setAbdid(const QString abdid);

    QString getBusienddate();
    void setBusienddate(const QString busiEnddate);

    QString getBusiday();
    void setBusiday(const QString busiDay);

    QString getBusirelaxday();
    void setBusirelaxday(const QString busiRelaxday);

    QString getBusifromadres();
    void setBusifromadres(const QString busiFromadres);

    QString getBusitoadres();
    void setBusitoadres(const QString busiToadres);

    QString getBusitranscost();
    void setBusitranscost(const QString busiTranscost);

    QString getBusitransbillnum();
    void setBusitransbillnum(const QString busiTransbillnum);

    QString getBusiaccomcost();
    void setBusiaccomcost(const QString busiAccomcost);

    QString getbusiAccombillnum();
    void setbusiAccombillnum(const QString busiAccombillnum);

    QString getBusitranssubsidy();
    void setBusitranssubsidy(const QString busiTranssubsidy);

    QString getBusifoodsubsidy();
    void setBusifoodsubsidy(const QString busiFoodsubsidy);

    QString getBusitotalcost();
    void setBusitotalcost(const QString busiTotalcost);

    QString getBusiothercost();
    void setBusiothercost(const QString busiOthercost);

    QString getBusiothernum();
    void setBusiothernum(const QString busiOthernum);

    QString getBusiothermemo();
    void setBusiothermemo(const QString busiOthermemo);

    void toString();

};

#endif // BUSIEXP_H
