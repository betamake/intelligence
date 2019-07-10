#include "billitem.h"
#include "ui_billitem.h"

billItem::billItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::billItem)
{
    ui->setupUi(this);

    mBill.init();

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

void billItem::on_modifyBtn_clicked()
{
    emit openBillItem(mBill);
}
