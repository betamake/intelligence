#include"abroadexp.h"
#include<QDebug>

abroadExp::abroadExp(){

}

abroadExp::~abroadExp(){

}

int abroadExp::getid(){
    return id;
}

void abroadExp::setid(const int id)
{
    this->id = id;
}

QString abroadExp::getAbdid(){
    return abdid;
}

void abroadExp::setAbdid(const QString abdid){
    this->abdid = abdid;
}

QString abroadExp::getAbdmanid()
{
    return abdManid;
}

void abroadExp::setAbdmanid(const QString abdManid)
{
    this->abdManid = abdManid;
}

QString abroadExp::getAbdvisacost()
{
    return abdVisabillnum;
}

void abroadExp::setAbdvisacost(const QString abdVisabillnum)
{
    this->abdVisabillnum =abdVisabillnum;
}

QString abroadExp::getAbdvisabillnum()
{
    return abdVisabillnum;
}

void abroadExp::setAbdvisabillnum(const QString abdVisabillnum)
{
    this->abdVisabillnum = abdVisabillnum;
}

QString abroadExp::getAbdservcost()
{
    return abdServcost;
}

void abroadExp::setAbdservcost(const QString abdServcost)
{
    this->abdServcost = abdServcost;
}

QString abroadExp::getAbdservbillnum()
{
    return abdServbillnum;
}

void abroadExp::setAbdservbillnum(const QString abdServbillnum)
{
    this->abdServbillnum = abdServbillnum;
}

QString abroadExp::getAbdexchgcost()
{
    return abdExchgcost;
}

void abroadExp::setAbdexchgcost(const QString abdExchgcost)
{
    this->abdExchgcost = abdExchgcost;
}

QString abroadExp::getAbdexchgbillnum()
{
    return abdExchgbillnum;
}

void abroadExp::setAbdexchgbillnum(const QString abdExchgbillnum)
{
    this->abdExchgbillnum = abdExchgbillnum;
}

void abroadExp::toString()
{

}
