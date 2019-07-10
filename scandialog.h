#ifndef SCANDIALOG_H
#define SCANDIALOG_H

#include <QDialog>
#include <QThread>
#include "scanthread.h"
#include "billcheck.h"
namespace Ui {
class scanDialog;
}
class billInfo{
public:
    QString getuser(){
        return this->user;
    }
    void setuser(const QString user){
        this->user =user;
    }
    QString getbillValue(){
        return this->billValue;
    }
    void setbillValue(const QString  billValue){
        this->billValue=billValue;
    }
    QString getbillNumber(){
        return this->billNumber;
    }
    void setbillNumber(const QString billNumber){
        this->billNumber=billNumber;
    }
    QString getbillContent(){
        return this->billContent;
    }
    void setbillContent(const QString billContent){
        this->billContent=billContent;
    }
    QString getbillRemark(){
        return this->billRemark;
    }
    void setbillRemark(const QString billRemark){
        this->billRemark=billRemark;
    }
    QString getsellerTitle(){
       return this->sellerTitle;
    }
    void setsellerTitle(const QString sellerTitle){
        this->sellerTitle =sellerTitle;
    }
    QString getsellerNumber(){
        return this->sellerNumber;
    }
    void setsellerNumber(const QString sellerNumber){
        this->sellerNumber=sellerNumber;
    }
    QString getsellerOpenBank(){
        return this->sellerOpenBank;
    }
    void setsellerOpenBank(const QString sellerOpenBank){
        this->sellerOpenBank=sellerOpenBank;
    }
    QString getsellerBankNumber(){
        return this->sellerBankNumber;
    }
    void setsellerBankNumber(const QString sellerBankNumber){
        this->sellerBankNumber=sellerBankNumber;
    }
    QString getsellerPhoneNumber(){
        return this->sellerPhoneNumber;
    }
    void setsellerPhoneNumber(const QString sellerPhoneNumber){
        this->sellerPhoneNumber=sellerPhoneNumber;
    }
    QString getsellerAdress(){
        return this->sellerAdress;
    }
    void setsellerAdress(const QString sellerAdress){
        this->sellerAdress=sellerAdress;
    }
    QString getbuyerTitle(){
       return this->buyerTitle;
    }
    void setbuyerTitle(const QString buyerTitle){
        this->buyerTitle =buyerTitle;
    }
    QString getbuyerrNumber(){
        return this->buyerNumber;
    }
    void setbuyerNumber(const QString buyerNumber){
        this->buyerNumber=buyerNumber;
    }
    QString getbuyerOpenBank(){
           return this->buyerOpenBank;
       }
       void setbuyerOpenBank(const QString buyerOpenBank){
           this->buyerOpenBank=buyerOpenBank;
       }
       QString getbuyerBankNumber(){
           return this->buyerBankNumber;
       }
       void setbuyerBankNumber(const QString buyerBankNumber){
           this->buyerBankNumber=buyerBankNumber;
       }
       QString getbuyerPhoneNumber(){
           return this->buyerPhoneNumber;
       }
       void setbuyerPhoneNumber(const QString buyerPhoneNumber){
           this->buyerPhoneNumber=buyerPhoneNumber;
       }
       QString getbuyerAdress(){
           return this->buyerAdress;
       }
       void setbuyerAdress(const QString buyerAdress){
           this->buyerAdress=buyerAdress;
       }

private:
    QString user;//当前用户',
    QString billValue;//票据金额',
    QString billNumber;//'票据号',
    QString billContent;// '发票内容',
    QString billRemark;// '发票备注',
    QString sellerTitle;//'销售方抬头',
    QString sellerNumber;//销售方税号',
    QString sellerOpenBank;//'销售方开户行',
    QString sellerBankNumber;// '销售方银行账号',
    QString sellerPhoneNumber;//'销售方电话',
    QString sellerAdress;//'销售方地址',
    QString buyerTitle;//'购买方抬头',
    QString buyerNumber;// '购买方税号',
    QString buyerOpenBank;//'购买方开户行',
    QString buyerBankNumber;// '购买方银行账号',
    QString buyerPhoneNumber;//'购买方电话',
    QString buyerAdress;//'购买方地址'
};
class scanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit scanDialog(QWidget *parent = 0);
    ~scanDialog();

    void setBillCheck(BillCheck info);

    void billInfoRead();//读取票据信息.
signals:
    void scanDone();
    void saveBillInfo(billInfo billinfo);//发送保存票据信息信号.


private slots:
    void on_buttonScan_clicked();
    void on_buttonClose_clicked();
    void dealDone();
    void dealThread();


private:
    Ui::scanDialog *ui;
    ScanThread *thread;

    BillCheck bill;
    billInfo billinfo;
};


#endif // SCANDIALOG_H
