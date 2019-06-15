#include "scandialog.h"
#include "ui_scandialog.h"

scanDialog::scanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scanDialog)
{
    ui->setupUi(this);
}

scanDialog::~scanDialog()
{
    delete ui;
}
