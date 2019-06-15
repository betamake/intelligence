#include "facemangedialog.h"
#include "ui_facemangedialog.h"

FaceMangeDialog::FaceMangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FaceMangeDialog)
{
    ui->setupUi(this);
}

FaceMangeDialog::~FaceMangeDialog()
{
    delete ui;
}
