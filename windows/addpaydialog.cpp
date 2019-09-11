#include "addpaydialog.h"
#include "ui_addpaydialog.h"
#include "windows/itemViews/paymethodsitem.h"
#include <QMessageBox>

addPayDialog::addPayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPayDialog)
{
    ui->setupUi(this);

    initInput();

    mIndex = -1;
    curItem = new payItemInfo();
    beginModify = false;
    currentType = 0;
    strUsage = "";
    ui->comboBox->setCurrentIndex(currentType);
    ui->stackedWidget->setCurrentIndex(currentType);

}

addPayDialog::~addPayDialog()
{
    delete ui;
}

void addPayDialog::on_comboBox_currentIndexChanged(int index)
{
    currentType = index;
    ui->stackedWidget->setCurrentIndex(index);
}

void addPayDialog::initInput()
{
    QRegExp rx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(rx, this);

    ui->bankAccount->setValidator(validator);
    ui->cardAccount->setValidator(validator);
    ui->cashAccount->setValidator(validator);
    ui->bankcardNumber->setValidator(validator);

    ui->cardDate->setCalendarPopup(true);
    QDate currentDate = QDate::currentDate();
    ui->cardDate->setDisplayFormat("yyyy/MM/dd");
    ui->cardDate->setDate(currentDate);
    ui->cardDate->setMaximumDate(currentDate);
}

//设置要修改的项目的内容
void addPayDialog::setItem(payItemInfo *info)
{
    int type = info->getType();
    if (type > 2 || type < 0)
        return;
    currentType = type;

    beginModify = true;

    ui->comboBox->setCurrentIndex(currentType);
    if (type == 0) {
        ui->bankAccount->setText(QString::number(info->getAccount()));
        ui->bankPayee->setText(info->getPayeeName());
        ui->bankProvince->setText(info->getProvince());
        ui->bankCity->setText(info->getCity());
        ui->bank->setText(info->getBank());
        ui->bankName->setText(info->getBankName());
        ui->bankcardNumber->setText(info->getBankcardNumber());
//        ui->bankUsage->setText(info->getUsage());
        ui->bankRemark->setText(info->getRemark());
    } else if (type == 1) {
        ui->cardAccount->setText(QString::number(info->getAccount()));
        ui->cardPayee->setText(info->getPayeeName());
        ui->cardDepartment->setText(info->getDepartment());
        ui->cardProvince->setText(info->getProvince());
        ui->cardCity->setText(info->getCity());
        ui->cardDate->setDate(info->getCardDate());
        ui->cardDealAccount->setText(QString::number(info->getCardAccount()));
        ui->cardBank->setText(info->getBank());
        ui->cardBankName->setText(info->getBankName());
        ui->cardBankcardNumber->setText(info->getBankcardNumber());
        ui->cardRemark->setText(info->getRemark());
    } else if (type == 2) {
        ui->cashAccount->setText(QString::number(info->getAccount()));
        ui->cashPayee->setText(info->getPayeeName());
        ui->cashRemark->setText(info->getRemark());
    }
}

//保存并退出按钮
void addPayDialog::on_saveEditBtn_clicked()
{
    curItem->clear();

    bool hasFixed = false;        //必填项已经填完

    if (currentType == 0) {
        if(ui->bankAccount->text().isEmpty()) {
            QMessageBox::warning(this, "warning", "请输入报销金额", QMessageBox::Ok);
        }
        else if(ui->bankPayee->text().isEmpty()) {
            QMessageBox::warning(this, "warning", "请输入收款人", QMessageBox::Ok);
        }
        else if (ui->bankName->text().isEmpty()) {
            QMessageBox::warning(this, "warning", "请填写开户行信息", QMessageBox::Ok);
        }
        else if (ui->bankcardNumber->text().isEmpty()) {
            QMessageBox::warning(this, "warning", "请填写银行卡号", QMessageBox::Ok);
        }
        else if (ui->bankUsageCom->currentIndex()==0) {
            QMessageBox::warning(this, "warning", "请选择用途", QMessageBox::Ok);
        } else {
            hasFixed = true;
            curItem->setType(0);
            curItem->setStrType("网银支付");
            curItem->setAccount(ui->bankAccount->text().toInt());
            curItem->setPayeeName(ui->bankPayee->text());
            curItem->setProvince(ui->bankProvince->text());
            curItem->setCity(ui->bankCity->text());
            curItem->setBank(ui->bank->text());
            curItem->setBankName(ui->bankName->text());
            curItem->setBankcardNumber(ui->bankcardNumber->text());
            curItem->setUsage(ui->bankUsageCom->currentText());
            curItem->setRemark(ui->bankRemark->toPlainText());
        }
    }
    else if (currentType == 1) {
        if ( ui->cardPayee->text().isEmpty()) {
            QMessageBox::warning(this, "warning", "请输入收款人", QMessageBox::Ok);
        }
        else if (ui->cardDealAccount->text().isEmpty()) {
            QMessageBox::warning(this, "warning", "请填写交易金额", QMessageBox::Ok);
        }
        else {
            hasFixed = true;
            curItem->setType(1);
            curItem->setStrType("公务卡支付");
            curItem->setAccount(ui->cardAccount->text().toInt());
            curItem->setPayeeName(ui->cardPayee->text());
            curItem->setDepartment(ui->cardDepartment->text());
            curItem->setProvince(ui->cardProvince->text());
            curItem->setCity(ui->cardCity->text());
            curItem->setCardDate(ui->cardDate->date());
            curItem->setCardDealDate(ui->cardDate->text());
            curItem->setCardAccount(ui->cardDealAccount->text().toInt());
            curItem->setBank(ui->cardBank->text());
            curItem->setBankName(ui->cardBankName->text());
            curItem->setBankcardNumber(ui->cardBankcardNumber->text());
            curItem->setRemark(ui->cardRemark->toPlainText());
        }
    }
    else if (currentType == 2) {
        if (ui->cashAccount->text().isEmpty() || ui->cashPayee->text().isEmpty()) {
            QMessageBox::warning(this, "warning", "还有信息未输入", QMessageBox::Ok);
        } else {
            hasFixed = true;
            curItem->setType(2);
            curItem->setStrType("现金支付");
            curItem->setAccount(ui->cashAccount->text().toInt());
            curItem->setPayeeName(ui->cashPayee->text());
            curItem->setRemark(ui->cashRemark->toPlainText());
        }
    }
    //必填项未填完时不关闭窗口
    if(!hasFixed)
        return;

    emit addPayItem(mIndex, curItem);
    close();
}

void addPayDialog::on_bankUsageCom_currentIndexChanged(int index)
{
    if (index == 0)
        strUsage = "";
    else if (index == 1)
        strUsage = "货款";
    else if (index == 2)
        strUsage = "报销";
}

void addPayDialog::on_searchPeopleButtop_clicked()
{
    allInterface::getinstance ()->info.setname (ui->bankPayee->text ());
    allInterface::getinstance ()->getuserdatalist ();
    connect (allInterface::getinstance (),SIGNAL(setUserDataListDone()),this,SLOT(dealPayInfo()));
}
void addPayDialog::dealPayInfo ()
{
    ui->bankProvince->setText (allInterface::getinstance ()->info.getprovice ());
    ui->bankCity->setText (allInterface::getinstance ()->info.getcity ());
    ui->bank->setText (allInterface::getinstance ()->info.getbankName ());
    ui->bankcardNumber->setText (allInterface::getinstance ()->info.getbankAccount ());
}

void addPayDialog::on_searchBtn_clicked()
{

}


