#include "addpaydialog.h"
#include "ui_addpaydialog.h"
#include "windows/itemViews/paymethodsitem.h"
#include "payinfomanager.h"

addPayDialog::addPayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPayDialog)
{
    ui->setupUi(this);

    curItem = new payItemInfo();
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

    payInfoManager::getInstance()->addPayItem(curItem);

    emit addPayItem(curItem);

    close();
}
