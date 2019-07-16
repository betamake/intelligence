#include "reimdetailitem.h"
#include "ui_reimdetailitem.h"
#include <QMessageBox>

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
    if (ui->budgetNumber->text().isEmpty() || ui->budgetName->text().isEmpty() || ui->items->text().isEmpty() ||
           ui->itemDetails->text().isEmpty() || ui->department->text().isEmpty() || ui->feeType->text().isEmpty() ||
            ui->reimAccount->text().isEmpty())  {
        QMessageBox::warning(this, "warning", "还有信息未输入", QMessageBox::Ok);
    }
    else {
        detail->setuser ("");
        detail->setBudgetNumber(ui->budgetNumber->text());
        detail->setBudgetName(ui->budgetName->text());
        detail->setItem(ui->items->text());
        detail->setDetails(ui->itemDetails->text());
        detail->setDepartment(ui->department->text());
        detail->setFeeType(ui->feeType->text());
        detail->setAccount(ui->reimAccount->text().toInt());
    }
}

void reimDetailItem::on_searchBudgetBtn_clicked()
{

}

void reimDetailItem::on_searchItemBtn_clicked()
{

}

void reimDetailItem::on_searchDepartmentBtn_clicked()
{

}
