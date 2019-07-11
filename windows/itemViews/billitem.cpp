#include "billitem.h"
#include "ui_billitem.h"
#include <QDebug>

billItem::billItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::billItem)
{
    ui->setupUi(this);

    mBill.init();
    iRow = -1;

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

void billItem::setIndex(int row)
{
    if (row < 0)
        qDebug() << "没有正确设置行号";
    iRow = row;
}

void billItem::on_modifyBtn_clicked()
{
    emit openBillItem(mBill);
}

void billItem::on_deleteBtn_clicked()
{
    emit deleteBillItem(iRow);
}
