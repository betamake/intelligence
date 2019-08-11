#include "reimdetailitem.h"
#include "ui_reimdetailitem.h"
#include <QMessageBox>
#include <QDoubleValidator>
#include <QString>
reimDetailItem::reimDetailItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reimDetailItem)
{
    detail = new reimDetail();
    ui->setupUi(this);

    ui->budgetName->setFocusPolicy(Qt::NoFocus);        //项目名称不可修改
    ui->itemDetails->setFocusPolicy(Qt::NoFocus);       //项目明细不可修改
    ui->feeType->setFocusPolicy(Qt::NoFocus);           //经费类型不可修改

    //浮点小数控制，这里有点问题，可以输入字母a-e，等待解决
    QDoubleValidator *validator = new QDoubleValidator();
    ui->reimAccount->setValidator(validator);

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
    QString budgetNumber = ui->budgetNumber->text ();
    allInterface::getinstance ()->info.setfundVersion (budgetNumber);
    allInterface::getinstance ()->getDataProject ();


}

void reimDetailItem::on_searchItemBtn_clicked()
{

}

void reimDetailItem::on_searchDepartmentBtn_clicked()
{

}
void reimDetailItem::setDataProjectInfo ()
{
    ui->budgetName->setText (allInterface::getinstance ()->info.getprojectName ());
    ui->feeType->setText (allInterface::getinstance ()->info.getmoneyOriginName ());
}
