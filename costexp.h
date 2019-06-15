#ifndef COSTEXP_H
#define COSTEXP_H
#include"iostream"
#include"QString"
#include "expensehead.h"
using namespace std;

class costExp
{
private:
    QString costid;
    QString costManid;//报销人id
    QString costMoney;//报销金额
    QString costBillamount;//单据张数
    QString costBudgtproid;//预算项目
    QString costCentrializedid;//预算归属部门
    QString costLoanid;//借款单
    QString costSurplusloan;//剩余借款
    QString costmemo;//备注
    //ExpenseHead cost;

public:
    costExp();
    ~costExp();

    QString getcostid();
    void setcostid(const QString costid);

    QString getCostmanid();
    void setCostmanid(const QString costManid);

    QString getCostmoney();
    void setCostmoney(const QString costMoney);

    QString getCostbillamount();
    void setCostbillamount(const QString costBillamount);

    QString getCostbudgtproid();
    void setCostbudgtproid(const QString costBudgtproid);

    QString getCostcentrializedid();
    void setCostcentrializedid(const QString costCentrializedid);

    QString getCostloanid();
    void setCostloanid(const QString costLoanid );

    QString getCostsurplusloan();
    void setCostsurplusloan(const QString costSurplusloan );

    QString getCostmemo();
    void setCostmemo(const QString costmemo);

    void toString();

};

#endif // COSTEXP_H
