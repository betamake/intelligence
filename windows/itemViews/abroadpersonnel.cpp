#include "abroadpersonnel.h"
#include "ui_abroadpersonnel.h"
#include <QMessageBox>

abroadPersonnel::abroadPersonnel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::abroadPersonnel)
{
    ui->setupUi(this);
    addListItems();
}

abroadPersonnel::~abroadPersonnel()
{
    delete ui;
}

void abroadPersonnel::setIndex(int index)
{
    ui->indexLab1->setNum(index);
    ui->indexLab2->setNum(index);
}

void abroadPersonnel::addListItems()
{

    for (int i=0; i<8; i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(721, 31));
        abroadFeeItem *feeItem = new abroadFeeItem();

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, feeItem);
    }
}

void abroadPersonnel::saveItem()
{
    if (ui->staffName->text().isEmpty() || ui->staffNumber->text().isEmpty() || ui->department->text().isEmpty() ||
           ui->startTime->text().isEmpty() || ui->endTime->text().isEmpty() || ui->days->text().isEmpty() ||
           ui->country->text().isEmpty() || ui->leaveCity->text().isEmpty() || ui->arriveCity->text().isEmpty() ||
           ui->certificateType->text().isEmpty() || ui->budgetNumEdit->text().isEmpty() || ui->budgetNameEdit->text().isEmpty() ){
        QMessageBox::warning(this, "warning", "还有信息未输入", QMessageBox::Ok);
    } else {
        //保存信息
    }
}
