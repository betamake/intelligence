#include "busiexpense.h"
#include <QDebug>
using namespace std;

BusiExpense::BusiExpense()
{
//    busiDept_line = "";
//    busiForm_line = "";
//    busiProject_line = "";
//    busiCause_line = "";
}
BusiExpense::~BusiExpense()
{
//        busiDept_line = "";
//        busiForm_line = "";
//        busiProject_line = "";
//        busiCause_line = "";
}
void BusiExpense::init()
{

//     busiDept_line = "";//经办人部门
//     busiForm_line= "";//借款单
//     busiProject_line= "";//支出项目
//     busiCause_line= "";//出差事由
    qDebug()<<"清除数据";
    setBusiCause_line("");
    setBusiDept_line("");
    setBusiForm_line("");
    setBusiProject_line("");
}
void BusiExpense::setBusiDept_line(const QString busiDept_line)
{
   this->busiDept_line = busiDept_line;
}
void BusiExpense::setBusiCause_line(const QString busiCause_line)
{
   this-> busiCause_line = busiCause_line;
}
void BusiExpense::setBusiForm_line(const QString busiForm_line)
{
   this-> busiForm_line = busiForm_line;
}
void BusiExpense::setBusiProject_line(const QString busiProject_line)
{
    this->busiProject_line = busiProject_line;
}
QString BusiExpense::getBusiDept_line()
{
    return busiDept_line;
}
QString BusiExpense::getBusiCause_line()
{
    return busiCause_line;
}
QString BusiExpense::getBusiForm_line()
{
    return busiForm_line;
}
QString BusiExpense::getBusiProject_line()
{
    return busiProject_line;
}
void BusiExpense::toString()
 {
     qDebug()<<"[busiDept_line:"<<busiDept_line<<", busiForm_line:"<<busiForm_line<<", busiProject_line:"<<busiProject_line<<", busiCause_line:"<<busiCause_line<<"]"<<endl;
 }
