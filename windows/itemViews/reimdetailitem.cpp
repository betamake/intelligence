#include "reimdetailitem.h"
#include "ui_reimdetailitem.h"

reimDetailItem::reimDetailItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reimDetailItem)
{
    ui->setupUi(this);
}

reimDetailItem::~reimDetailItem()
{
    delete ui;
}
