#include "abroadexpense.h"
#include <QDebug>

abroadExpense::abroadExpense()
{/*
    QString abordDept;//经办人部门
    QString abordContraNum;//合同号
    QString abordContra;//合同名称
    QString abordForm;//借款单
    QString abordCause;//用途
    QString abordType_comboBox;//费用类型*/
}
QString abroadExpense::getAbordDept()
{
    return abordDept;
}
void abroadExpense::setAbordContraNum(const QString abordContraNum)
{
    this->abordContraNum = abordContraNum;
}
QString abroadExpense::getAbordContra()
{
    return abordContra;
}
void abroadExpense::setAbordContra(const QString abordContra)
{
    this->abordContra = abordContra;
}
void abroadExpense::setAbordForm(const QString abordForm)
{
    this->abordForm = abordForm;
}
void abroadExpense::setAbordDept(const QString abordDept)
{
    this->abordDept = abordDept;
}
QString abroadExpense::getAbordForm()
{
    return abordForm;
}
void abroadExpense::setAbordCause(const QString abordCause)
{
    this->abordCause = abordCause;
}
QString abroadExpense::getAbordCause()
{
    return abordCause;
}
void abroadExpense::setAbordType(const QString abordType)
{
    this->abordType = abordType;
}
QString abroadExpense::getAbordType()
{
    return abordType;
}
void abroadExpense::toString()
{
    qDebug()<<"[abordDept:"<<abordDept<<", abordContraNum:"<<abordContraNum
           <<", abordContra:"<<abordContra<<", abordForm:"<<abordForm<<", abordCause:"<<abordCause<<",abordType:"<<abordType<<"]"<<endl;
}
