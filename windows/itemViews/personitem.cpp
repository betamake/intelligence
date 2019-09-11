#include "personitem.h"
#include "ui_personitem.h"

personItem::personItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::personItem)
{
    ui->setupUi(this);
}

personItem::~personItem()
{
    delete ui;
}

void personItem::setPersonName(QString &strName)
{
    ui->personName->setText(strName);
}

void personItem::setAccount(int account)
{
    ui->account->setNum(account);
}

void personItem::setDeparture(QString &departure)
{
    ui->departure->setText(departure);
}

void personItem::setDestination(QString &destination)
{
    ui->destination->setText(destination);
}
