#include "addpaydialog.h"
#include "ui_addpaydialog.h"
#include "windows/itemViews/paymethodsitem.h"
#include "payinfomanager.h"

addPayDialog::addPayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPayDialog)
{
    ui->setupUi(this);

    initInput();

    curItem = new payItemInfo();
    beginModify = false;
    currentType = 0;
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
        ui->bankUsage->setText(info->getUsage());
        ui->bankRemark->setText(info->getRemark());
    } else if (type == 1) {
        ui->cardAccount->setText(QString::number(info->getAccount()));
        ui->cardPayee->setText(info->getPayeeName());
        ui->cardDepartment->setText(info->getDepartment());
        ui->cardProvince->setText(info->getProvince());
        ui->cardCity->setText(info->getCity());
        ui->cardDealDate->setText(info->getCardDealDate());
        ui->cardDealAccount->setText(QString::number(info->getCardAccount()));
        ui->cardBank->setText(info->getBank());
        ui->cardBankName->setText(info->getBankName());
        ui->cardBankcardNumber->setText(info->getBankcardNumber());
        ui->cardUsage->setText(info->getUsage());
        ui->cardRemark->setText(info->getRemark());
    } else if (type == 2) {
        ui->cashAccount->setText(QString::number(info->getAccount()));
        ui->cashPayee->setText(info->getPayeeName());
        ui->cashUsage->setText(info->getUsage());
        ui->cashRemark->setText(info->getRemark());
    }
}

//保存并退出按钮
void addPayDialog::on_saveEditBtn_clicked()
{
    curItem->clear();
    if (currentType == 0) {
        curItem->setType(0);
        curItem->setAccount(ui->bankAccount->text().toInt());
        curItem->setPayeeName(ui->bankPayee->text());
        curItem->setProvince(ui->bankProvince->text());
        curItem->setCity(ui->bankCity->text());
        curItem->setBank(ui->bank->text());
        curItem->setBankName(ui->bankName->text());
        curItem->setBankcardNumber(ui->bankcardNumber->text());
        curItem->setUsage(ui->bankUsage->text());
        curItem->setRemark(ui->bankRemark->toPlainText());
    }
    else if (currentType == 1) {
        curItem->setType(1);
        curItem->setAccount(ui->cardAccount->text().toInt());
        curItem->setPayeeName(ui->cardPayee->text());
        curItem->setDepartment(ui->cardDepartment->text());
        curItem->setProvince(ui->cardProvince->text());
        curItem->setCity(ui->cardCity->text());
        curItem->setCardDealDate(ui->cardDealDate->text());
        curItem->setCardAccount(ui->cardDealAccount->text().toInt());
        curItem->setBank(ui->cardBank->text());
        curItem->setBankName(ui->cardBankName->text());
        curItem->setBankcardNumber(ui->cardBankcardNumber->text());
        curItem->setUsage(ui->cardUsage->text());
        curItem->setRemark(ui->cardRemark->toPlainText());
    }
    else if (currentType == 2) {
        curItem->setType(2);
        curItem->setAccount(ui->cashAccount->text().toInt());
        curItem->setPayeeName(ui->cashPayee->text());
        curItem->setUsage(ui->cashUsage->text());
        curItem->setRemark(ui->cashRemark->toPlainText());
    }

    if (true == beginModify) {
        //在payInfoManager中也要修改对应的支付信息

        emit modifyPayItem(curItem);
        beginModify = false;
    } else {
        payInfoManager::getInstance()->addPayItem(curItem);
        emit addPayItem(curItem);
    }

    close();
}
