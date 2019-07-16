#include "abroadfeeitem.h"
#include "ui_abroadfeeitem.h"

abroadFeeItem::abroadFeeItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::abroadFeeItem)
{
    ui->setupUi(this);
    getType();
    initInput();
    rmb = 0;

    QComboBox *comboBox = ui->comboBox;
    connect(comboBox, &QComboBox::currentTextChanged, this, &abroadFeeItem::getType);
    QLineEdit *waibi = ui->waibi;
    connect(waibi, &QLineEdit::editingFinished, this, &abroadFeeItem::countValue);
}

abroadFeeItem::~abroadFeeItem()
{
    delete ui;
}

void abroadFeeItem::getType()
{
    QString feeType = ui->comboBox->currentText();
    if (feeType == "美元")
        mHuilv = 6.65;
    else if(feeType == "英镑")
        mHuilv = 10.1;

    ui->huilv->setText(QString::number(mHuilv, 10, 3));
}

double abroadFeeItem::countValue()
{
//    double rmb = 0;
    QString str = ui->waibi->text();
    double val = str.toDouble();
    rmb = val * mHuilv;
    ui->rmb->setText(QString::number(rmb, 10, 4));

    return rmb;
}

void abroadFeeItem::sendValue()
{
    emit sendFee(rmb);
}

void abroadFeeItem::clearInput()
{
    ui->waibi->clear();
    ui->rmb->clear();
}

void abroadFeeItem::initInput()
{
    ui->waibi->clear();
    ui->rmb->clear();

    QRegExp rx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(rx, this);

    //浮点小数控制，这里有点问题，可以输入字母a-e，等待解决
//    QDoubleValidator *doubleValidator = new QDoubleValidator();
//    ui->waibi->setValidator(doubleValidator);
//    ui->rmb->setValidator(doubleValidator);

    ui->waibi->setValidator(validator);
    ui->rmb->setValidator(validator);

    //人民币金额框不允许输入
    ui->rmb->setFocusPolicy(Qt::NoFocus);
}
