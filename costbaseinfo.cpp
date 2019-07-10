#include "costbaseinfo.h"
#include <database.h>
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
