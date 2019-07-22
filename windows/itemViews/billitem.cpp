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

    billType = 1;
    ui->detailWidget->hide();       //默认隐藏站点信息

    ui->accountLabel->setText("");
    ui->typeLabel->setText("");
}

billItem::~billItem()
{
    delete ui;
}

void billItem::setBillType(const int type)
{
    QString typeStr = "发票";
    if (type == 1) {
        typeStr = "发票";
        ui->detailWidget->hide();
    }
    else if (type ==2) {
        typeStr = "火车票";
        ui->detailWidget->show();
    }
    else if (type == 3) {
        typeStr = "飞机票";
        ui->detailWidget->show();
    }
    else if (type == 4) {
        typeStr = "出租车票";
        ui->detailWidget->hide();
    }

    ui->typeLabel->setText(typeStr);
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
