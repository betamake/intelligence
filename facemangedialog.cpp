#include "facemangedialog.h"
#include "ui_facemangedialog.h"

FaceMangeDialog::FaceMangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FaceMangeDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("人脸库管理"));
}

FaceMangeDialog::~FaceMangeDialog()
{
    delete ui;
}
