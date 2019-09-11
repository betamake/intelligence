#include "scandialog.h"
#include "ui_scandialog.h"
#include <QDebug>
#include <QPixmap>
#include <QTime>
#include "database.h"
scanDialog::scanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scanDialog)
{
    ui->setupUi(this);
    setWindowTitle ("扫描票据");
    thread = new ScanThread(this);
    database *dataset = new database(this);
    connect (this,&scanDialog::destroyed,this,&scanDialog::dealThread);
    connect (this,SIGNAL(saveBillInfo(billInfo)),dataset,SLOT(insertBillInfo(billInfo)));
}



scanDialog::~scanDialog()
{
    delete ui;
}

void scanDialog::setBillCheck(BillCheck info)
{
    ui->accountEdit->setText(info.getBillmoney());
    ui->typeEdit->setText(info.getBilltype());
    ui->lineEdit_2->setText (info.getBillnumber ());
    ui->lineEdit_3->setText (info.getBillcontent ());
}

void scanDialog::on_buttonScan_clicked()
{
    qDebug() << "开始扫描";
    ui->label->setText ("开始扫描");
    ui->buttonScan->setText ("再添加一张");
    thread->start ();
    connect (thread,&ScanThread::isDone,this,&scanDialog::dealDone);
    QThread::sleep (2);
    QPixmap pix("/home/intelligence/桌面/build-expenseRobot_v1-Desktop_Qt_5_8_0_GCC_64bit-Debug/finalbill.jpg");
    QPixmap dest = pix.scaled(ui->label_32->size (),Qt::KeepAspectRatio);
    ui->label_32->setPixmap (dest);
    emit scanDone ();
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
//读取票据信息
void scanDialog::billInfoRead ()
{
    billinfo.setuser ("");
    billinfo.setbillValue ("ui->lineEdit->text ()");
    billinfo.setbillNumber (ui->lineEdit_2->text ());
    billinfo.setbillContent (ui->lineEdit_3->text ());
    billinfo.setbillRemark ("ui->lineEdit_4->text ()");
    billinfo.setsellerTitle (ui->lineEdit_5->text ());
    billinfo.setsellerNumber (ui->lineEdit_6->text ());
    billinfo.setsellerOpenBank (ui->lineEdit_7->text ());
    billinfo.setsellerBankNumber (ui->lineEdit_8->text ());
    billinfo.setsellerPhoneNumber (ui->lineEdit_9->text ());
    billinfo.setsellerAdress (ui->lineEdit_10->text ());
    billinfo.setbuyerTitle (ui->lineEdit_24->text ());
    billinfo.setbuyerNumber (ui->lineEdit_26->text ());
    billinfo.setbuyerOpenBank (ui->lineEdit_27->text ());
    billinfo.setbuyerBankNumber (ui->lineEdit_23->text ());
    billinfo.setbuyerPhoneNumber (ui->lineEdit_25->text ());
    billinfo.setbuyerAdress (ui->lineEdit_28->text ());
    emit saveBillInfo (billinfo);
}
//备用
//void scanDialog::billInfoRead ()
//{
//    billinfo.setuser ("");
//    billinfo.setbillValue ("");
//    billinfo.setbillNumber ("");
//    billinfo.setbillContent ("");
//    billinfo.setbillRemark ("");
//    billinfo.setsellerTitle ("");
//    billinfo.setsellerNumber ("");
//    billinfo.setsellerOpenBank ("");
//    billinfo.setsellerBankNumber ("");
//    billinfo.setsellerPhoneNumber ("");
//    billinfo.setsellerAdress ("");
//    billinfo.setbuyerTitle ("");
//    billinfo.setbuyerNumber ("");
//    billinfo.setbuyerOpenBank ("");
//    billinfo.setbuyerBankNumber ("");
//    billinfo.setbuyerPhoneNumber ("");
//    billinfo.setbuyerAdress ("");
//}


void scanDialog::on_ExitButton_clicked()
{
    this->dealDone ();
    close();
}
