#include "scandialog.h"
#include "ui_scandialog.h"
#include <QDebug>
#include <QPixmap>
#include <QTime>
scanDialog::scanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scanDialog)
{
    ui->setupUi(this);
    setWindowTitle ("扫描票据");
    thread = new ScanThread(this);
    connect (this,&scanDialog::destroyed,this,&scanDialog::dealThread);
}



scanDialog::~scanDialog()
{
    delete ui;
}

void scanDialog::on_buttonScan_clicked()
{
    qDebug() << "开始扫描";
    ui->label->setText ("开始扫描");
    thread->start ();
    connect (thread,&ScanThread::isDone,this,&scanDialog::dealDone);
    QThread::sleep (2);
    QPixmap pix("/home/intelligence/桌面/build-expenseRobot_v1-Desktop_Qt_5_8_0_GCC_64bit-Debug/finalbill.jpg");
    QPixmap dest = pix.scaled(ui->label_32->size (),Qt::KeepAspectRatio);
    ui->label_32->setPixmap (dest);
    emit scanDone ();

}



void scanDialog::on_buttonClose_clicked()
{
    this->dealDone ();
}

void scanDialog::dealDone ()
{
    thread->quit ();
    thread->wait ();

}
void scanDialog::dealThread ()
{
    thread->quit ();
    thread->wait ();

}
