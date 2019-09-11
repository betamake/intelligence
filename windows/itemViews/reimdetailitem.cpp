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
//    ui->itemDetails->setFocusPolicy(Qt::NoFocus);       //项目明细不可修改
    ui->feeType->setFocusPolicy(Qt::NoFocus);           //经费类型不可修改

    //浮点小数控制，这里有点问题，可以输入字母a-e，等待解决
    QDoubleValidator *validator = new QDoubleValidator();
    ui->reimAccount->setValidator(validator);
}

reimDetailItem::~reimDetailItem()
{
    delete detail;
    delete ui;
}

void reimDetailItem::setReimDetail(reimDetail *info)
 {
    detail = info;
    ui->reimAccount->setText(QString::number(info->getAccount()));
}

void reimDetailItem::saveDetail()
{
    if (ui->budgetNumber->text().isEmpty())
        QMessageBox::warning(this, "warning", "请填写预算项目号", QMessageBox::Ok);
    else if (ui->budgetName->text().isEmpty())
        QMessageBox::warning(this, "warning", "请填写预算项目名", QMessageBox::Ok);
    else if (ui->items->text().isEmpty())
        QMessageBox::warning(this, "warning", "请填写业务事项", QMessageBox::Ok);
    else if (ui->itemDetails->text().isEmpty())
        QMessageBox::warning(this, "warning", "请填写项目明细", QMessageBox::Ok);
    else if (ui->department->text().isEmpty())
        QMessageBox::warning(this, "warning", "请填写使用科室", QMessageBox::Ok);
    else if (ui->feeType->text().isEmpty())
        QMessageBox::warning(this, "warning", "请填写经费类型", QMessageBox::Ok);
    else if (ui->reimAccount->text().isEmpty())
        QMessageBox::warning(this, "warning", "请输入金额", QMessageBox::Ok);
    else {
        detail->setuser ("");
        detail->setBudgetNumber(ui->budgetNumber->text());
        detail->setBudgetName(ui->budgetName->text());
        detail->setItem(ui->items->text());
        detail->setDetails(ui->itemDetails->text());
        detail->setDepartment(ui->department->text());
        detail->setFeeType(ui->feeType->text());
        detail->setAccount(ui->reimAccount->text().toInt());
//        emit hadSaved(detail);
    }
}

void reimDetailItem::on_searchBudgetBtn_clicked()
{
    QString budgetNumber = ui->budgetNumber->text ();
    detail->setBudgetNumber(budgetNumber);
    allInterface::getinstance ()->info.setfundVersion (budgetNumber);
    allInterface::getinstance ()->info.setprojectName (ui->budgetName->text ());
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
    QString strBudgetName = allInterface::getinstance ()->info.getprojectName ();
    QString strFeeType = allInterface::getinstance ()->info.getmoneyOriginName ();

    ui->budgetName->setText (strBudgetName);
    ui->feeType->setText (strFeeType);

    detail->setBudgetName(strBudgetName);
    detail->setFeeType(strFeeType);
}

void reimDetailItem::on_reimAccount_editingFinished()
{
    if (ui->budgetNumber->text().isEmpty())
        QMessageBox::warning(this, "warning", "请填写预算项目号", QMessageBox::Ok);
//    else if (ui->budgetName->text().isEmpty())
//        QMessageBox::warning(this, "warning", "请填写预算项目名", QMessageBox::Ok);
    else if (ui->items->text().isEmpty())
        QMessageBox::warning(this, "warning", "请填写业务事项", QMessageBox::Ok);
//    else if (ui->itemDetails->text().isEmpty())
//        QMessageBox::warning(this, "warning", "请填写项目明细", QMessageBox::Ok);
    else if (ui->department->text().isEmpty())
        QMessageBox::warning(this, "warning", "请填写使用科室", QMessageBox::Ok);
//    else if (ui->feeType->text().isEmpty())
//        QMessageBox::warning(this, "warning", "请填写经费类型", QMessageBox::Ok);
    else if (ui->reimAccount->text().isEmpty())
        QMessageBox::warning(this, "warning", "请输入金额", QMessageBox::Ok);
    else
    {
        detail->setuser ("");
        detail->setBudgetNumber(ui->budgetNumber->text());
        detail->setBudgetName(ui->budgetName->text());
        detail->setItem(ui->items->text());
        detail->setDetails(ui->itemDetails->text());
        detail->setDepartment(ui->department->text());
        detail->setFeeType(ui->feeType->text());
        detail->setAccount(ui->reimAccount->text().toInt());

        emit changeAccount();
    }

//    if (ui->reimAccount->text().isEmpty())
//        detail->setAccount(0);
//    else
//        detail->setAccount(ui->reimAccount->text().toInt());
//    emit changeAccount();
}
