#include<iostream>
using namespace std;
#include <QString>
#ifndef BUSIEXPENSE_H
#define BUSIEXPENSE_H
#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif


class BusiExpense
{
public:
    BusiExpense();
    ~BusiExpense();

    QString getBusiDept_line();
    void setBusiDept_line(const QString busiDept);

    QString getBusiForm_line();
    void setBusiForm_line(const QString busiForm);

    QString getBusiProject_line();
    void setBusiProject_line(const QString busiProject);

    QString getBusiCause_line();
    void setBusiCause_line(const QString sbusiCause);

    void toString();
    void init();

private:
    QString busiDept_line;//经办人部门
    QString busiForm_line;//借款单
    QString busiProject_line;//支出项目
    QString busiCause_line;//出差事由
};

#endif // BUSIEXPENSE_H
