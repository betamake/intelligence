#ifndef COSTEXPENSE_H
#define COSTEXPENSE_H
#include<QString>
#include<iostream>
using namespace std;

class costExpense
{
public:
    costExpense();
    ~costExpense();

    QString getCostDept();
    void setCostDept(const QString costDept);

    QString getCostContraNum();
    void setCostContraNum(const QString costContraNum);

    QString getCostContra();
    void setCostContra(const QString costContra);

    QString getCostForm();
    void setCostForm(const QString costForm);

    QString getCostUse();
    void setCostUse(const QString costUse);

    QString getCostType_comboBox();
    void setCostType_comboBox(const QString costType_comboBox);

    void toString();

private:
    QString costDept;//经办人部门
    QString costContraNum;//合同号
    QString costContra;//合同名称
    QString costForm;//借款单
    QString costUse;//用途
    QString costType_comboBox;//费用类型
};

#endif // COSTEXPENSE_H
