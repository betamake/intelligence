#include"busiexp.h"
#include<QDebug>

busiExp::busiExp()
{

}

busiExp::~busiExp()
{

}
 void busiExp::setEmpty ()
 {
     setAbdid ("");
     setbusiAccombillnum ("");
     setBusiaccomcost ("");
     setBusibegindate ("");
     setbusid ("");
     setBusiday ("");
     setBusienddate ("");
     setBusifoodsubsidy ("");
     setBusimanid ("");
     setBusiothercost ("");
     setBusiothermemo ("");
     setBusiothernum ("");
     setBusirelaxday ("");
     setBusitoadres ("");
     setBusitotalcost ("");
     setBusitransbillnum ("");
     setBusitranscost ("");
    setBusitranssubsidy ("");
 }
//void busiExp::init()
//{
//     busiManid = "";//出差人
//     busid = "";//差旅报销单号
//     abdid = "";//出国单号
//     busiBegindate= "";//起始时间
//     busiEnddate= "";//终止时间
//     busiDay= "";//天数
//     busiRelaxday= "";//公休天数
//     busiFromadres= "";//出发地点
//     busiToadres= "";//到达地点
//     busiTranscost= "";//城市间交通费
//     busiTransbillnum= "";//交通费单据张数
//     busiAccomcost= "";//住宿费用
//     busiAccombillnum= "";//住宿单据张数
//     busiTranssubsidy= "";//交通补贴
//     busiFoodsubsidy= "";//伙食补贴
//     busiTotalcost= "";//小计
//     busiOthercost= "";//其他费用金额
//     busiOthernum= "";//其他费用单据张数
//     busiOthermemo= "";//其他费用说明
//}
QString busiExp::getBusimanid()
{
    return busiManid;
}

void busiExp::setBusimanid(const QString busiManid)
{
    this->busiManid = busiManid;
}

QString busiExp::getbusid(){
    return busid;
}

void busiExp::setbusid(const QString busid){
    this->busid = busid;
}

QString busiExp::getAbdid(){
    return abdid;
}

void busiExp::setAbdid(const QString abdid)
{
    this->abdid = abdid;
}

QString busiExp::getBusibegindate()
{
    return busiBegindate;
}

void busiExp::setBusibegindate(const QString busiBegindate)
{
    this->busiBegindate = busiBegindate;
}

QString busiExp::getBusienddate()
{
    return busiEnddate;
}

void busiExp::setBusienddate(const QString busiEnddate)
{
    this->busiEnddate =busiEnddate;
}

QString busiExp::getBusiday()
{
    return busiDay;
}

void busiExp::setBusiday(const QString busiDay)
{
    this->busiDay = busiDay;
}

QString busiExp::getBusirelaxday()
{
    return busiRelaxday;
}

void busiExp::setBusirelaxday(const QString busiRelaxday)
{
    this->busiRelaxday = busiRelaxday;
}

QString busiExp::getBusifromadres()
{
    return busiFromadres;
}

void busiExp::setBusifromadres(const QString busiFromadres)
{
    this->busiFromadres = busiFromadres;
}

QString busiExp::getBusitoadres()
{
    return busiToadres;
}

void busiExp::setBusitoadres(const QString busiToadres)
{
    this->busiToadres = busiToadres;
}

QString busiExp::getBusitranscost()
{
    return busiTranscost;
}

void busiExp::setBusitranscost(const QString busiTranscost)
{
    this->busiTranscost = busiTranscost;
}

QString busiExp::getBusitransbillnum()
{
    return busiTransbillnum;
}

void busiExp::setBusitransbillnum(const QString busiTransbillnum)
{
    this->busiTransbillnum = busiTransbillnum;
}

QString busiExp::getBusiaccomcost()
{
    return busiAccomcost;
}

void busiExp::setBusiaccomcost(const QString busiAccomcost)
{
    this->busiAccomcost = busiAccomcost;
}

QString busiExp::getbusiAccombillnum()
{
    return busiAccombillnum;
}

void busiExp::setbusiAccombillnum(const QString busiAccombillnum)
{
    this->busiAccombillnum = busiAccombillnum;
}

QString busiExp::getBusitranssubsidy()
{
    return busiTranssubsidy;
}

void busiExp::setBusitranssubsidy(const QString busiTranssubsidy)
{
    this->busiTranssubsidy = busiTranssubsidy;
}

QString busiExp::getBusifoodsubsidy()
{
    return busiFoodsubsidy;
}

void busiExp::setBusifoodsubsidy(const QString busiFoodsubsidy)
{
    this->busiFoodsubsidy = busiFoodsubsidy;
}

QString busiExp::getBusitotalcost()
{
    return busiTotalcost;
}

void busiExp::setBusitotalcost(const QString busiTotalcost)
{
    this->busiTotalcost = busiTotalcost;
}

QString busiExp::getBusiothercost()
{
    return busiOthercost;
}

void busiExp::setBusiothercost(const QString busiOthercost)
{
    this->busiOthercost = busiOthercost;
}

QString busiExp::getBusiothernum()
{
    return busiOthernum;
}

void busiExp::setBusiothernum(const QString busiOthernum)
{
    this->busiOthernum = busiOthernum;
}

QString busiExp::getBusiothermemo()
{
    return busiOthermemo;
}

void busiExp::setBusiothermemo(const QString busiOthermemo)
{
    this->busiOthermemo = busiOthermemo;
}

void busiExp::toString()
{

}
