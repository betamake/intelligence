#include "costbaseinfo.h"
#include <databaseutils.h>
costBaseInfo::costBaseInfo(QWidget *parent) : QWidget(parent)
{

}

void costBase::setEmpty ()
{
    this->setcostConname ("");
    this->setcostCono ("");
    this->setcostHandp ("");
    this->setcostHandpd ("");
    this->setcostRdate ("");
    this->setcostRnumEidt ("");
    this->setcostUse ("");
}
