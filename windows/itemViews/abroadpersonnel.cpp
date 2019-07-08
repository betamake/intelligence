#include "abroadpersonnel.h"
#include "ui_abroadpersonnel.h"

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

