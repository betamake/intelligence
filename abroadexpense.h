#ifndef ABROADEXPENSE_H
#define ABROADEXPENSE_H
#include<iostream>
using namespace std;
#include <QString>
#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif
class abroadExpense
{
public:
    abroadExpense();
    QString getAbordDept();
    void setAbordDept(const QString abordDept);

    QString getAbordContraNum();
    void setAbordContraNum(const QString abordContraNum);

    QString getAbordContra();
    void setAbordContra(const QString abordContra);

    QString getAbordForm();
    void setAbordForm(const QString abordForm);

    QString getAbordType();
    void setAbordType(const QString abordType);

    QString getAbordCause();
    void setAbordCause(const QString abordCause);

    void toString();

private:
    QString abordDept;//经办人部门
    QString abordContraNum;//合同号
    QString abordContra;//合同名称
    QString abordForm;//借款单
    QString abordCause;//用途
    QString abordType;//费用类型
};

#endif // ABROADEXPENSE_H
