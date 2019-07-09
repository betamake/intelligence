#include "billitem.h"
#include "ui_billitem.h"

billItem::billItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::billItem)
{
    ui->setupUi(this);

    ui->accountLabel->setText("");
    ui->typeLabel->setText("");
}

billItem::~billItem()
{
    delete ui;
}

void billItem::setBillType(const QString &str)
{
    ui->typeLabel->setText(str);
}

void billItem::setBillAccount(const QString &money)
{
    ui->accountLabel->setText(money);
}

void billItem::setBillPixmap(QPixmap image)
{
    ui->label->setPixmap(image);
}
