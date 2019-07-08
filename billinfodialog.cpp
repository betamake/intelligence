#include "billinfodialog.h"
#include "ui_billinfodialog.h"

billinfodialog::billinfodialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::billinfodialog)
{
    ui->setupUi(this);
}

billinfodialog::~billinfodialog()
{
    delete ui;
}

void billinfodialog::showimg(QTableWidgetItem *img)
{
//    ui->billimage->setText(img->text());
}

void billinfodialog::showbill(QString billpath)
{
//    QImage img(billpath);
//    QPixmap pixmap = QPixmap::fromImage(img);
//    pixmap.scaled(ui->billimage->size(),Qt::KeepAspectRatio);
//    ui->billimage->setScaledContents(true);
//    ui->billimage->setPixmap(pixmap);

}

