#include "abroadpersonnel.h"
#include "ui_abroadpersonnel.h"
#include <QMessageBox>

abroadPersonnel::abroadPersonnel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::abroadPersonnel)
{
    ui->setupUi(this);
//    addListItems();
    initInput();

    abroadPerInfo = new abroadPersonInfo();

    getCoinType();

    connect(ui->comboBox1, &QComboBox::currentTextChanged, this, &abroadPersonnel::getCoinType);
    connect(ui->comboBox2, &QComboBox::currentTextChanged, this, &abroadPersonnel::getCoinType);
    connect(ui->comboBox3, &QComboBox::currentTextChanged, this, &abroadPersonnel::getCoinType);
    connect(ui->comboBox4, &QComboBox::currentTextChanged, this, &abroadPersonnel::getCoinType);
    connect(ui->comboBox5, &QComboBox::currentTextChanged, this, &abroadPersonnel::getCoinType);
    connect(ui->comboBox6, &QComboBox::currentTextChanged, this, &abroadPersonnel::getCoinType);
    connect(ui->comboBox7, &QComboBox::currentTextChanged, this, &abroadPersonnel::getCoinType);
    connect(ui->comboBox8, &QComboBox::currentTextChanged, this, &abroadPersonnel::getCoinType);

    connect(ui->waibi1, &QLineEdit::editingFinished, this, &abroadPersonnel::setValue);
    connect(ui->waibi2, &QLineEdit::editingFinished, this, &abroadPersonnel::setValue);
    connect(ui->waibi3, &QLineEdit::editingFinished, this, &abroadPersonnel::setValue);
    connect(ui->waibi4, &QLineEdit::editingFinished, this, &abroadPersonnel::setValue);
    connect(ui->waibi5, &QLineEdit::editingFinished, this, &abroadPersonnel::setValue);
    connect(ui->waibi6, &QLineEdit::editingFinished, this, &abroadPersonnel::setValue);
    connect(ui->waibi7, &QLineEdit::editingFinished, this, &abroadPersonnel::setValue);
    connect(ui->waibi8, &QLineEdit::editingFinished, this, &abroadPersonnel::setValue);
}

abroadPersonnel::~abroadPersonnel()
{
    delete ui;
}

void abroadPersonnel::setIndex(int index)
{
    ui->indexLab1->setNum(index);
    ui->indexLab2->setNum(index);
}

void abroadPersonnel::initInput()
{
    //浮点小数控制，这里有点问题，可以输入字母a-e，等待解决
//    QDoubleValidator *doubleValidator = new QDoubleValidator();
//    ui->waibi->setValidator(doubleValidator);
//    ui->rmb->setValidator(doubleValidator);

    QRegExp rx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(rx, this);

    ui->waibi1->setValidator(validator);
    ui->waibi2->setValidator(validator);
    ui->waibi3->setValidator(validator);
    ui->waibi4->setValidator(validator);
    ui->waibi5->setValidator(validator);
    ui->waibi6->setValidator(validator);
    ui->waibi7->setValidator(validator);
    ui->waibi8->setValidator(validator);

    ui->rmb1->setValidator(validator);
    ui->rmb2->setValidator(validator);
    ui->rmb3->setValidator(validator);
    ui->rmb4->setValidator(validator);
    ui->rmb5->setValidator(validator);
    ui->rmb6->setValidator(validator);
    ui->rmb7->setValidator(validator);
    ui->rmb8->setValidator(validator);

    //人民币金额框不允许输入
    ui->rmb1->setFocusPolicy(Qt::NoFocus);
    ui->rmb2->setFocusPolicy(Qt::NoFocus);
    ui->rmb3->setFocusPolicy(Qt::NoFocus);
    ui->rmb4->setFocusPolicy(Qt::NoFocus);
    ui->rmb5->setFocusPolicy(Qt::NoFocus);
    ui->rmb6->setFocusPolicy(Qt::NoFocus);
    ui->rmb7->setFocusPolicy(Qt::NoFocus);
    ui->rmb8->setFocusPolicy(Qt::NoFocus);

    //汇率框不允许输入
    ui->huilv1->setFocusPolicy(Qt::NoFocus);
    ui->huilv2->setFocusPolicy(Qt::NoFocus);
    ui->huilv3->setFocusPolicy(Qt::NoFocus);
    ui->huilv4->setFocusPolicy(Qt::NoFocus);
    ui->huilv5->setFocusPolicy(Qt::NoFocus);
    ui->huilv6->setFocusPolicy(Qt::NoFocus);
    ui->huilv7->setFocusPolicy(Qt::NoFocus);
    ui->huilv8->setFocusPolicy(Qt::NoFocus);
}

void abroadPersonnel::getCoinType()
{
    QString feeType = "";
    double huilv = 0.0;

    feeType = ui->comboBox1->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv1->setText(QString::number(huilv, 10, 3));

    feeType = ui->comboBox2->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv2->setText(QString::number(huilv, 10, 3));

    feeType = ui->comboBox3->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv3->setText(QString::number(huilv, 10, 3));

    feeType = ui->comboBox4->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv4->setText(QString::number(huilv, 10, 3));

    feeType = ui->comboBox5->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv5->setText(QString::number(huilv, 10, 3));

    feeType = ui->comboBox6->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv6->setText(QString::number(huilv, 10, 3));

    feeType = ui->comboBox7->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv7->setText(QString::number(huilv, 10, 3));

    feeType = ui->comboBox8->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv8->setText(QString::number(huilv, 10, 3));
}

double abroadPersonnel::chooseCoinType(QString &str)
{
    if ("美元" == str)
        return 6.65;
    else if("英镑" == str)
        return 10.1;
    return 0;
}

void abroadPersonnel::setValue()
{
    double val = 0;
    double huilv = 0;
    double rmb = 0;

    val = ui->waibi1->text().toDouble();
    huilv = ui->huilv1->text().toDouble();
    rmb = val * huilv;
    ui->rmb1->setText(QString::number(rmb, 10, 4));

    val = ui->waibi2->text().toDouble();
    huilv = ui->huilv2->text().toDouble();
    rmb = val * huilv;
    ui->rmb2->setText(QString::number(rmb, 10, 4));

    val = ui->waibi3->text().toDouble();
    huilv = ui->huilv3->text().toDouble();
    rmb = val * huilv;
    ui->rmb3->setText(QString::number(rmb, 10, 4));

    val = ui->waibi4->text().toDouble();
    huilv = ui->huilv4->text().toDouble();
    rmb = val * huilv;
    ui->rmb4->setText(QString::number(rmb, 10, 4));

    val = ui->waibi5->text().toDouble();
    huilv = ui->huilv5->text().toDouble();
    rmb = val * huilv;
    ui->rmb5->setText(QString::number(rmb, 10, 4));

    val = ui->waibi6->text().toDouble();
    huilv = ui->huilv6->text().toDouble();
    rmb = val * huilv;
    ui->rmb6->setText(QString::number(rmb, 10, 4));

    val = ui->waibi7->text().toDouble();
    huilv = ui->huilv7->text().toDouble();
    rmb = val * huilv;
    ui->rmb7->setText(QString::number(rmb, 10, 4));

    val = ui->waibi8->text().toDouble();
    huilv = ui->huilv8->text().toDouble();
    rmb = val * huilv;
    ui->rmb8->setText(QString::number(rmb, 10, 4));

}

//void abroadPersonnel::addListItems()
//{
//    for (int i=0; i<8; i++) {
//        QListWidgetItem *item = new QListWidgetItem;
//        item->setSizeHint(QSize(721, 31));
//        abroadFeeItem *feeItem = new abroadFeeItem();

//        ui->listWidget->addItem(item);
//        ui->listWidget->setItemWidget(item, feeItem);

//        connect(this, &abroadPersonnel::addFee, feeItem, &abroadFeeItem::sendValue);
//        connect(feeItem, &abroadFeeItem::sendFee, this, &abroadPersonnel::addOneFee);
//    }
//}


void abroadPersonnel::saveItem()
{
    if (ui->staffName->text().isEmpty() || ui->staffNumber->text().isEmpty() || ui->department->text().isEmpty() ||
           ui->startTime->text().isEmpty() || ui->endTime->text().isEmpty() || ui->days->text().isEmpty() ||
           ui->country->text().isEmpty() || ui->leaveCity->text().isEmpty() || ui->arriveCity->text().isEmpty() ||
           ui->certificateType->text().isEmpty() || ui->budgetNumEdit->text().isEmpty() || ui->budgetNameEdit->text().isEmpty() ){
        QMessageBox::warning(this, "warning", "还有信息未输入", QMessageBox::Ok);
    } else {
        //保存信息
        abroadPerInfo->setStaffName(ui->staffName->text());
        abroadPerInfo->setStaffNumber(ui->staffNumber->text().toInt());
        abroadPerInfo->setDepartment(ui->department->text());
        abroadPerInfo->setLeaveDate(ui->startTime->text());
        abroadPerInfo->setReturnDate(ui->endTime->text());
        abroadPerInfo->setCountry(ui->country->text());
        abroadPerInfo->setLeaveCity(ui->leaveCity->text());
        abroadPerInfo->setArriveCity(ui->arriveCity->text());
        abroadPerInfo->setCertificateType(ui->certificateType->text());
        abroadPerInfo->setBudgetNum(ui->budgetNumEdit->text().toInt());
        abroadPerInfo->setBudgetName(ui->budgetNameEdit->text());
        abroadPerInfo->setBudgetType(ui->budgetTypeEdit->text());

        double rmb1 = ui->rmb1->text().toDouble();
        double rmb2 = ui->rmb1->text().toDouble();
        double rmb3 = ui->rmb1->text().toDouble();
        double rmb4 = ui->rmb1->text().toDouble();
        double rmb5 = ui->rmb1->text().toDouble();
        double rmb6 = ui->rmb1->text().toDouble();
        double rmb7 = ui->rmb1->text().toDouble();
        double rmb8 = ui->rmb1->text().toDouble();

        double total = rmb1 + rmb2 + rmb3 + rmb4 + rmb5 + rmb6 + rmb7 + rmb8;

        abroadPerInfo->setTotalFee(total);


        //将人员信息的结构体保存起来
        personnelManager::getInstance()->addAbroadItem(abroadPerInfo);

        emit added();

    }
}
