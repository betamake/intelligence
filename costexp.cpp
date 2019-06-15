#include"costexp.h"
#include<QDebug>

costExp::costExp()
{

}

costExp::~costExp()
{

}

QString costExp::getcostid(){
    return costid;
}

void costExp::setcostid(const QString costid){
    this->costid = costid;
}

QString costExp::getCostmanid()
{
    return costManid;
}

void costExp::setCostmanid(const QString costManid)
{
    this->costManid = costManid;
}

QString costExp::getCostmoney()
{
    return costMoney;
}

void costExp::setCostmoney(const QString costMoney)
{
    this->costMoney = costMoney;
}

QString costExp::getCostbillamount()
{
    return costBillamount;
}

void costExp::setCostbillamount(const QString costBillamount)
{
    this->costBillamount = costBillamount;
}

QString costExp::getCostbudgtproid()
{
    return costBudgtproid;
}

void costExp::setCostbudgtproid(const QString costBudgtproid)
{
    this->costBudgtproid = costBudgtproid;
}

QString costExp::getCostcentrializedid()
{
    return costCentrializedid;
}

void costExp::setCostcentrializedid(const QString costCentrializedid)
{
    this->costCentrializedid = costCentrializedid;
}

QString costExp::getCostloanid()
{
    return costLoanid;
}

void costExp::setCostloanid(const QString costLoanid )
{
    this->costLoanid = costLoanid;
}

QString costExp::getCostsurplusloan()
{
    return costSurplusloan;
}

void costExp::setCostsurplusloan(const QString costSurplusloan )
{
    this->costSurplusloan = costSurplusloan;
}

QString costExp::getCostmemo()
{
    return costmemo;
}

void costExp::setCostmemo(const QString costmemo)
{
    this->costmemo = costmemo;
}

void costExp::toString()
{

}
