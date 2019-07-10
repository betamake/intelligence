#include "payinfomanager.h"

payInfoManager *payInfoManager::instance = NULL;

void payItemInfo::clear()
{
    this->type = 0;
    this->account = 0;
    this->payeeName = "";
    this->department = "";
    this->province = "";
    this->city = "";
    this->bank = "";
    this->bankName = "";
    this->bankcardNumber = "";
    this->usage = "";
    this->remark = "";
    this->cardDealDate = "";
    this->cardAccount = 0;
}

payInfoManager::payInfoManager(QObject *parent) : QObject(parent)
{
    payList.clear();
    itemNumber = 0;
}

payInfoManager *payInfoManager::getInstance()
{
    if (NULL == instance) {
        instance = new payInfoManager();
    }
    return instance;
}

payInfoManager::~payInfoManager()
{
    delete instance;
    instance = NULL;
}

void payInfoManager::delCurrentItem()
{

}

void payInfoManager::addPayItem(payItemInfo *info)     //添加支付信息
{
    payList.append(info);

    emit addPayItemView(info);
}

void payInfoManager::deletePayItem(payItemInfo *info)
{

}

void payInfoManager::clearPayItem(payItemInfo *info)
{
    //info->clear();
}

QList<payItemInfo*> payInfoManager::getPayList()
{
    return payList;
}
void payInfoManager::payInfoRead (){
//    payinfo.setAccount ();
//    payinfo.setBank ();
//    payinfo.setBankcardNumber ();
//    payinfo.setBankName ();
//    payinfo.setCardAccount ();
//    payinfo.setCardDealDate ();
//    payinfo.setCity ();
//    payinfo.setDepartment ();
//    payinfo.setPayeeName ();
//    payinfo.setProvince ();
//    payinfo.setRemark ();
//    payinfo.setType ();
//    payinfo.setUsage ();
}
