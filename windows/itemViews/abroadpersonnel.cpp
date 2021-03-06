#include "abroadpersonnel.h"
#include "ui_abroadpersonnel.h"
#include <QMessageBox>
#include <QDebug>

abroadPersonnel::abroadPersonnel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::abroadPersonnel)
{
    ui->setupUi(this);

    initInput();
    getCoinType();

    mIndex = -1;
    abroadPerInfo = new abroadPersonInfo();

    connect(ui->leaveDateEdit, &QDateEdit::dateChanged, this, &abroadPersonnel::getDays);
    connect(ui->backDateEdit, &QDateEdit::dateChanged, this, &abroadPersonnel::getDays);

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
    mIndex = index;
    ui->indexLab1->setNum(index);
    ui->indexLab2->setNum(index);
}

void abroadPersonnel::getDays()
{
    QDate date1 = ui->leaveDateEdit->date();
    QDate date2 = ui->backDateEdit->date();

    qint64 days = date1.daysTo(date2);
    if(days < 0) {
        QMessageBox::warning(this, "waring", "启程时间不能比返回时间后", QMessageBox::Ok);
        ui->days->setText("0");
    } else {
        qDebug() << "days大于或等于0" << endl;
        ui->days->setText(QString::number(days));
    }
}

void abroadPersonnel::initInput()
{
    //设置日期控件
    ui->leaveDateEdit->setCalendarPopup(true);
    ui->backDateEdit->setCalendarPopup(true);
    //设置日期显示形式
    ui->leaveDateEdit->setDisplayFormat("yyyy/MM/dd");
    ui->backDateEdit->setDisplayFormat("yyyy/MM/dd");
    //设置为当天时间
    QDate date = QDate::currentDate();
    ui->leaveDateEdit->setDate(date);
    ui->backDateEdit->setDate(date);
    //设置最大日期
    ui->leaveDateEdit->setMaximumDate(date);
    ui->backDateEdit->setMaximumDate(date);

    ui->days->setText("0");
    ui->days->setFocusPolicy(Qt::NoFocus);

    //浮点小数控制，这里有点问题，可以输入字母a-e，等待解决
    QDoubleValidator *validator = new QDoubleValidator();

    //设置只能输入Int类型
//    QRegExp rx("[0-9]+$");
//    QValidator *validator = new QRegExpValidator(rx, this);

    ui->waibi1->setValidator(validator);
    ui->waibi2->setValidator(validator);
    ui->waibi3->setValidator(validator);
    ui->waibi4->setValidator(validator);
    ui->waibi5->setValidator(validator);
    ui->waibi6->setValidator(validator);
    ui->waibi7->setValidator(validator);
    ui->waibi8->setValidator(validator);

//    //汇率框不允许输入
//    ui->huilv1->setFocusPolicy(Qt::NoFocus);
//    ui->huilv2->setFocusPolicy(Qt::NoFocus);
//    ui->huilv3->setFocusPolicy(Qt::NoFocus);
//    ui->huilv4->setFocusPolicy(Qt::NoFocus);
//    ui->huilv5->setFocusPolicy(Qt::NoFocus);
//    ui->huilv6->setFocusPolicy(Qt::NoFocus);
//    ui->huilv7->setFocusPolicy(Qt::NoFocus);
//    ui->huilv8->setFocusPolicy(Qt::NoFocus);

    //人民币金额框不允许输入
    ui->rmb1->setFocusPolicy(Qt::NoFocus);
    ui->rmb2->setFocusPolicy(Qt::NoFocus);
    ui->rmb3->setFocusPolicy(Qt::NoFocus);
    ui->rmb4->setFocusPolicy(Qt::NoFocus);
    ui->rmb5->setFocusPolicy(Qt::NoFocus);
    ui->rmb6->setFocusPolicy(Qt::NoFocus);
    ui->rmb7->setFocusPolicy(Qt::NoFocus);
    ui->rmb8->setFocusPolicy(Qt::NoFocus);

    ui->totalFee->setNum(0);
}

//设置汇率
void abroadPersonnel::getCoinType()
{
    QString feeType = "";
    double huilv = 0.0;

    feeType = ui->comboBox1->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv1->setText(QString::number(huilv, 10, 4));

    feeType = ui->comboBox2->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv2->setText(QString::number(huilv, 10, 4));

    feeType = ui->comboBox3->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv3->setText(QString::number(huilv, 10, 4));

    feeType = ui->comboBox4->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv4->setText(QString::number(huilv, 10, 4));

    feeType = ui->comboBox5->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv5->setText(QString::number(huilv, 10, 4));

    feeType = ui->comboBox6->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv6->setText(QString::number(huilv, 10, 4));

    feeType = ui->comboBox7->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv7->setText(QString::number(huilv, 10, 4));

    feeType = ui->comboBox8->currentText();
    huilv = chooseCoinType(feeType);
    ui->huilv8->setText(QString::number(huilv, 10, 4));
}

double abroadPersonnel::chooseCoinType(QString &str)
{
    if ("美元" == str)
        return 6.65;
    else if("英镑" == str)
        return 10.1;
    return 0;
}

//计算人民币金额
void abroadPersonnel::setValue()
{
    double val = 0;
    double huilv = 0;
    double rmb = 0;
    double total = 0;

    val = ui->waibi1->text().toDouble();
    huilv = ui->huilv1->text().toDouble();
    rmb = val * huilv;
    ui->rmb1->setText(QString::number(rmb, 10, 2));
    total += rmb;

    val = ui->waibi2->text().toDouble();
    huilv = ui->huilv2->text().toDouble();
    rmb = val * huilv;
    ui->rmb2->setText(QString::number(rmb, 10, 2));
    total += rmb;

    val = ui->waibi3->text().toDouble();
    huilv = ui->huilv3->text().toDouble();
    rmb = val * huilv;
    ui->rmb3->setText(QString::number(rmb, 10, 2));
    total += rmb;

    val = ui->waibi4->text().toDouble();
    huilv = ui->huilv4->text().toDouble();
    rmb = val * huilv;
    ui->rmb4->setText(QString::number(rmb, 10, 2));
    total += rmb;

    val = ui->waibi5->text().toDouble();
    huilv = ui->huilv5->text().toDouble();
    rmb = val * huilv;
    ui->rmb5->setText(QString::number(rmb, 10, 2));
    total += rmb;

    val = ui->waibi6->text().toDouble();
    huilv = ui->huilv6->text().toDouble();
    rmb = val * huilv;
    ui->rmb6->setText(QString::number(rmb, 10, 2));
    total += rmb;

    val = ui->waibi7->text().toDouble();
    huilv = ui->huilv7->text().toDouble();
    rmb = val * huilv;
    ui->rmb7->setText(QString::number(rmb, 10, 2));
    total += rmb;

    val = ui->waibi8->text().toDouble();
    huilv = ui->huilv8->text().toDouble();
    rmb = val * huilv;
    ui->rmb8->setText(QString::number(rmb, 10, 2));
    total += rmb;

    ui->totalFee->setNum(total);
}

void abroadPersonnel::saveItem()
{
    if (ui->staffName->text().isEmpty())
        QMessageBox::warning(this, "warning", "请输入员工姓名", QMessageBox::Ok);
    else if (ui->staffNumber->text().isEmpty())
        QMessageBox::warning(this, "warning", "请输入员工工号", QMessageBox::Ok);
    else if (ui->department->text().isEmpty())
        QMessageBox::warning(this, "warning", "请输入部门", QMessageBox::Ok);
    else if (ui->leaveCity->text().isEmpty())
        QMessageBox::warning(this, "warning", "请输入出发城市", QMessageBox::Ok);
    else if (ui->arriveCity->text().isEmpty())
        QMessageBox::warning(this, "warning", "请输入到达城市", QMessageBox::Ok);
    else if (ui->budgetNumEdit->text().isEmpty())
        QMessageBox::warning(this, "warning", "请输入项目经费号", QMessageBox::Ok);
    else if (ui->budgetNameEdit->text().isEmpty() ){
        QMessageBox::warning(this, "warning", "请输入项目名", QMessageBox::Ok);
    }
    else {
        //保存信息
        abroadPerInfo->setStaffName(ui->staffName->text());
        abroadPerInfo->setStaffNumber(ui->staffNumber->text().toInt());
        abroadPerInfo->setDepartment(ui->department->text());
        abroadPerInfo->setLeaveDate(ui->leaveDateEdit->text());
        abroadPerInfo->setLeaveDate1(ui->leaveDateEdit->date());
        abroadPerInfo->setReturnDate(ui->backDateEdit->text());
        abroadPerInfo->setReturnDate1(ui->backDateEdit->date());
        abroadPerInfo->setLeaveCity(ui->leaveCity->text());
        abroadPerInfo->setArriveCity(ui->arriveCity->text());
//        abroadPerInfo->setCertificateType(ui->certificateType->text());
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

        addToMainWindow(mIndex, abroadPerInfo);

//        //将人员信息的结构体保存起来
//        personnelManager::getInstance()->addAbroadItem(abroadPerInfo);
//        emit added();

    }
}

void abroadPersonnel::on_searchStaffBtn_clicked()
{

}

void abroadPersonnel::on_staffNumber_editingFinished()
{
    QString staffname = ui->staffNumber->text ();
    allInterface::getinstance ()->info.setname (staffname);
    allInterface::getinstance ()->getuserdatalist ();
    connect (allInterface::getinstance (),SIGNAL(setUserDataListDone()),this,SLOT(dealUserData()));
}
void abroadPersonnel::dealUserData ()
{
    ui->department->setText (allInterface::getinstance ()->getinfo ().getofficeName ());
}


void abroadPersonnel::on_arriveCity_editingFinished()
{
    allInterface::getinstance ()->getinfo ().setarriveCity (ui->arriveCity->text ());
    allInterface::getinstance ()->getdataCountriesExpense ();
    connect (allInterface::getinstance (),SIGNAL(setdataCountriesExpenseDone()),this,SLOT(dealdataCountriesExpense()));
}
void abroadPersonnel::dealdataCountriesExpense ()
{
    qDebug()<<"注册费"<<allInterface::getinstance ()->getinfo ().getaboradStaySubsidy ();
    qDebug()<<"公杂费"<<allInterface::getinstance ()->getinfo ().getcommonMiscellaneous ();
    qDebug()<<"伙食费"<<allInterface::getinstance ()->getinfo ().getaboradMealSubsidy ();
    qDebug()<<"币种"<<allInterface::getinstance ()->getinfo ().getcurrencyName ();

}
void abroadPersonnel::setPerson(abroadPersonInfo *info)
{
    ui->staffName->setText(info->getStaffName());
//    ui->staffNumber->setText(QString::number(info->getStaffNumber()));
    ui->department->setText(info->getDepartment());

    QDate leaveDate = info->getLeaveDate1();
    QDate backDate = info->getReturnDate1();
    qint64 days = leaveDate.daysTo(backDate);
    ui->leaveDateEdit->setDate(leaveDate);
    ui->backDateEdit->setDate(backDate);
    ui->days->setText(QString::number(days));

    ui->leaveCity->setText(info->getLeaveCity());
    ui->arriveCity->setText(info->getArriveCity());

    ui->budgetNumEdit->setText(QString::number(info->getBudgetNum()));
    ui->budgetNameEdit->setText(info->getBudgetName());
    ui->budgetTypeEdit->setText(info->getBudgetType());
}
