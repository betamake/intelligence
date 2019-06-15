#ifndef EXPENSEHEAD_H
#define EXPENSEHEAD_H
#include"iostream"
#include"QString"
#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif
using namespace std;
//#include <QObject>

class ExpenseHead
{
    //Q_OBJECT
public:
    //explicit ExpenseHead(QObject *parent = 0);
    ExpenseHead();
    ~ExpenseHead();

    QString getHeadbusid();
    void setHeadbusid(const QString headBusid);

    QString getHeadcostid();
    void setHeadcostid(const QString headCostid);

    QString getHeadcode();
    void setHeadcode(const QString headCode);

    QString getHeaddeptid();
    void setHeaddeptid(const QString headDeptid);

    QString getHeadoperatorid();
    void setHeadoperatori(const QString headOperatorid);

    QString getHeadDate();
    void setHeaddate(const QString headDate);

    QString getHeadyear();
    void setHeadyear(const QString headYear);

    QString getHeadpurpose();
    void setHeadpurpose(const QString headPurpose);

    QString getHeadrequisitionid();
    void setHeadrequisitionid(const QString headRequisitionid);

    QString getHeadloanmoneysum();
    void setHeadloanmoneysum(const QString headLoanmoneysum);

    QString getHeadprojectid();
    void setHeadprojectid(const QString headProjectid);

    QString getHeadcurrency();
    void setHeadcurrency(const QString headCurrency);

    QString getHeadExrate();
    void setHeadExrate(const QString headExrate);

    QString getHeadeinvoiceid();
    void setHeadeinvoiceid(const QString headEinvoiceid);

    QString getHeadCosttype();
    void setHeadCosttype(const QString headCosttype);

    QString getHeadcontractid();
    void setHeadcontractid(const QString headContractid);

    QString getHeadContractname();
    void setHeadContractname(const QString headContractname);

    void toString();

private:
    QString headBusid;//差旅报销单号
    QString headCostid;//费用报销单号
    QString headCode;//报销单编号
    QString headDeptid;//经办部门
    QString headOperatorid;//经办人
    QString headDate;//单据日期
    QString headYear;//归属年度
    QString headPurpose;//出差事由、用途
    QString headRequisitionid;//借款单id
    QString headLoanmoneysum;//冲借款金额
    QString headProjectid;//支出项目
    QString headCurrency;//币种
    QString headExrate;//汇率
    QString headEinvoiceid;//电子发票号
    QString headCosttype;//费用类型
    QString headContractid;//合同号
    QString headContractname;//合同名称

//signals:

//public slots:

};

#endif // EXPENSEHEAD_H
