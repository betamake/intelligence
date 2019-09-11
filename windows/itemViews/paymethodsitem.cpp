#include "paymethodsitem.h"
#include "ui_paymethodsitem.h"


payMethodsItem::payMethodsItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::payMethodsItem)
{
    curItem = new payItemInfo();
    ui->setupUi(this);
}

payMethodsItem::~payMethodsItem()
{
    delete ui;
}

void payMethodsItem::setInfoItem(payItemInfo *info)
{
    int type = info->getType();
    if (type > 2 || type < 0)
        return;
    curItem = info;
    if (type == 0) {
        ui->methodLabel->setText("网银支付");
        ui->accountLabel->setNum(info->getAccount());
    }
    else if (type == 1) {
        ui->methodLabel->setText("公务卡支付");
        ui->accountLabel->setNum(info->getAccount());
    }
    else if (type == 2) {
        ui->methodLabel->setText("现金支付");
        ui->accountLabel->setNum(info->getAccount());
    }
}
