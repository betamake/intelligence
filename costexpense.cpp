#include "costexpense.h"
#include <QDebug>

costExpense::costExpense()
{
//    QString costDept;//经办人部门
//    QString costContraNum;//合同号
//    QString costContra;//合同名称
//    QString costForm;//借款单
//    QString costUse;//用途
//    QString costType_comboBox;//费用类型
}
costExpense::~costExpense()
{
//    QString costDept = "";//经办人部门
//    QString costContraNum="";//合同号
//    QString costContra="";//合同名称
//    QString costForm="";//借款单
//    QString costUse="";//用途
//    QString costType_comboBox="";//费用类型
}

QString costExpense::getCostContraNum()
{
    return costContraNum;
}
void costExpense::setCostContraNum(const QString costContraNum)
{
    this->costContraNum = costContraNum;
}
QString costExpense::getCostContra()
{
    return costContra;
}
void costExpense::setCostContra(const QString costContra)
{
    this->costContra = costContra;
}
QString costExpense::getCostDept()
{
    return costDept;
}
void costExpense::setCostForm(const QString costForm)
{
    this->costForm = costForm;
}
void costExpense::setCostDept(const QString costDept)
{
    this->costDept = costDept;
}
QString costExpense::getCostForm()
{
    return costForm;
}
void costExpense::setCostUse(const QString costUse)
{
    this->costUse = costUse;
}
QString costExpense::getCostUse()
{
    return costUse;
}
void costExpense::setCostType_comboBox(const QString costType_comboBox)
{
    this->costType_comboBox = costType_comboBox;
}
QString costExpense::getCostType_comboBox()
{
    return costType_comboBox;
}
void costExpense::toString()
{
    qDebug()<<"[costDept:"<<costDept<<", costContraNum:"<<costContraNum
           <<", costContra:"<<costContra<<", costForm:"<<costForm<<", costUse:"<<costUse<<",costType_comboBox:"<<costType_comboBox<<"]"<<endl;
}
