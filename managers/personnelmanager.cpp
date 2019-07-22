#include "personnelmanager.h"

abroadPersonInfo::abroadPersonInfo()
{
    setEmpty();
}

void abroadPersonInfo::setEmpty()
{
    this->staffName = "";
    this->staffNumber = 0;
    this->department = "";
    this->leaveDate = "";
    this->returnDate = "";
//    this->country = "";
    this->leaveCity = "";
    this->arriveCity = "";
    this->certificateType = "";

    this->budgetNum = 0;
    this->budgetName = "";
    this->budgetType = "";

    this->totalFee = 0;
}

traBusPersonInfo::traBusPersonInfo()
{
    setEmpty();
}

void traBusPersonInfo::setEmpty()
{
    this->staffName = "";
    this->staffNumber = 0;
    this->leaveDate = "";
    this->returnDate = "";
    this->days = 0;
    this->department = "";
    this->leavePlace = "";
    this->arrivePlace = "";

    this->budgetNum = 0;
    this->budgetName = "";
    this->budgetType = "";

    this->cityFee = 0;
    this->stayFee = 0;
    this->workFee = 0;
    this->lunchDays = 0;
    this->lunchSubsidy = 0;
    this->airportFee = 0;
    this->traDays = 0;
    this->traSubsidy = 0;
    this->otherFee = 0;

    this->totalFee = 0;
}

personnelManager *personnelManager::instance = NULL;

personnelManager::personnelManager()
{

}

personnelManager *personnelManager::getInstance()
{
    if (NULL == instance) {
        instance = new personnelManager();
    }
    return instance;
}

personnelManager::~personnelManager()
{
    delete instance;
    instance = NULL;
}
