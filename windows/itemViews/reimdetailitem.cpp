#include "reimdetailitem.h"
#include "ui_reimdetailitem.h"

reimDetailItem::reimDetailItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reimDetailItem)
{
    detail = new reimDetail();
    ui->setupUi(this);
}

reimDetailItem::~reimDetailItem()
{
    delete ui;
}

void reimDetailItem::saveDetail()
{
    detail->setuser ("");
    detail->setBudgetNumber(ui->budgetNumber->text());
    detail->setBudgetName(ui->budgetName->text());
    detail->setItem(ui->items->text());
    detail->setDetails(ui->itemDetails->text());
    detail->setDepartment(ui->department->text());
    detail->setFeeType(ui->feeType->text());
    detail->setAccount(ui->reimAccount->text().toInt());
}
