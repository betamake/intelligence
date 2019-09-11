#include "insertpersonneldialog.h"
#include "ui_insertpersonneldialog.h"

#include <QDebug>
#include <QMessageBox>

insertPersonnelDialog::insertPersonnelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::insertPersonnelDialog)
{
    ui->setupUi(this);

    initItemView();

    traPerson = new traBusPersonInfo();
    abroadPerson = new abroadPersonInfo();
    mDays = 0;

    mIndex = -1;
    //默认为出差人员，默认是院内人员
    personType = 1;
    ui->stackedWidget->setCurrentIndex(0);
    isInSchool = true;

    //出差标准
    lunchPerDay = 0;
    travelPerDay = 0;
    stayPerDay = 0;

    //出国
    aboradStaySubsidy = 0; //国外注册费用
    commonMiscellaneous = 0;//公杂费
    aboradMealSubsidy = 0;//国外伙食标准

    connectItems();
}

insertPersonnelDialog::~insertPersonnelDialog()
{
    disconnectItems();
    delete ui;
}

//初始化时的一些控件上的配置，信号和槽connect写在构造函数中
void insertPersonnelDialog::initItemView()
{
    //是否社内人员选择
    groupBtn = new QButtonGroup(this);
    groupBtn->addButton(ui->yesRadioBtn, 0);
    groupBtn->addButton(ui->noRadioBtn, 1);
    ui->yesRadioBtn->setChecked(true);          //默认是社内人员
    isInSchool = true;

    //第一页的日期选择
    //首先设置下拉选择日期
    ui->leaveDateEdit->setCalendarPopup(true);
    ui->backDateEdit->setCalendarPopup(true);
    //设置日期显示形式
    ui->leaveDateEdit->setDisplayFormat("yyyy/MM/dd");
    ui->backDateEdit->setDisplayFormat("yyyy/MM/dd");
    //给日期框分配当天的日期
    QDate date = QDate::currentDate();
    ui->leaveDateEdit->setDate(date);
    ui->backDateEdit->setDate(date);

    //设置最大日期
    ui->leaveDateEdit->setMaximumDate(date);
    ui->backDateEdit->setMaximumDate(date);

    //出差天数，补贴天数初始化为0
    ui->busiDays->setText("0");
    ui->busiDays->setFocusPolicy(Qt::NoFocus);      //出差天数不允许手动修改
    ui->lunchDays->setText("0");
    ui->travelDays->setText("0");
    //各种补贴初始化为0
    ui->cityTraFee->setText("0");
    ui->stayFee->setText("0");
    ui->workFee->setText("0");
    ui->lunchSubsidy->setText("0");
    ui->airportFee->setText("0");
    ui->travelSubsidy->setText("0");
    ui->otherFees->setText("0");
    ui->totalFee->setNum(0);

    //设置lineEdit只能输入数字，以免出现问题，目前只能输入整数，小数等之后完善
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, this);
    ui->budgetNumEdit->setValidator(validator);         //预算项目号
    ui->cityTraFee->setValidator(validator);            //城市间交通费
    ui->stayFee->setValidator(validator);               //住宿费
    ui->workFee->setValidator(validator);               //会务费/注册费

    ui->lunchDays->setValidator(new QIntValidator(0, 1000, this));             //伙食补贴天数
    ui->lunchSubsidy->setValidator(validator);          //伙食补贴费
    ui->airportFee->setValidator(validator);            //往返机场交通费
    ui->travelDays->setValidator(validator);            //交通补贴天数
    ui->travelSubsidy->setValidator(validator);         //交通补贴费
    ui->otherFees->setValidator(validator);             //其他费用


    //第二页的日期选择
    //设置日期控件
    ui->leaveDate->setCalendarPopup(true);
    ui->backDate->setCalendarPopup(true);
    //设置日期显示形式
    ui->leaveDate->setDisplayFormat("yyyy/MM/dd");
    ui->backDate->setDisplayFormat("yyyy/MM/dd");
//    //设置为当天时间
    ui->leaveDate->setDate(date);
    ui->backDate->setDate(date);
    //设置最大日期setIsInSchool
    ui->leaveDate->setMaximumDate(date);
    ui->backDate->setMaximumDate(date);

    ui->days->setText("0");
    ui->days->setFocusPolicy(Qt::NoFocus);

    //浮点小数控制，这里有点问题，可以输入字母e，等待解决
    QDoubleValidator *doublevalidator = new QDoubleValidator();

    ui->waibi1->setValidator(doublevalidator);
    ui->waibi2->setValidator(doublevalidator);
    ui->waibi3->setValidator(doublevalidator);
    ui->waibi4->setValidator(doublevalidator);
    ui->waibi5->setValidator(doublevalidator);
    ui->waibi6->setValidator(doublevalidator);
    ui->waibi7->setValidator(doublevalidator);
    ui->waibi8->setValidator(doublevalidator);

    ui->huilv1->setValidator(doublevalidator);
    ui->huilv2->setValidator(doublevalidator);
    ui->huilv3->setValidator(doublevalidator);
    ui->huilv4->setValidator(doublevalidator);
    ui->huilv5->setValidator(doublevalidator);
    ui->huilv6->setValidator(doublevalidator);
    ui->huilv7->setValidator(doublevalidator);
    ui->huilv8->setValidator(doublevalidator);

    //人民币金额框不允许输入
    ui->rmb1->setFocusPolicy(Qt::NoFocus);
    ui->rmb2->setFocusPolicy(Qt::NoFocus);
    ui->rmb3->setFocusPolicy(Qt::NoFocus);
    ui->rmb4->setFocusPolicy(Qt::NoFocus);
    ui->rmb5->setFocusPolicy(Qt::NoFocus);
    ui->rmb6->setFocusPolicy(Qt::NoFocus);
    ui->rmb7->setFocusPolicy(Qt::NoFocus);
    ui->rmb8->setFocusPolicy(Qt::NoFocus);

    ui->totalFee_2->setNum(0);
}

void insertPersonnelDialog::connectItems()
{
    //选择是否为院内人员
    connect(groupBtn, SIGNAL(buttonClicked(int)), this, SLOT(isSchool(int)));

    //修改日期的时候，自动更新出差天数和补贴天数
    connect(ui->leaveDateEdit, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
    connect(ui->backDateEdit, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
    connect(ui->leaveDate, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
    connect(ui->backDate, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);

    //修改补贴天数的时候，要确认天数是否合理
    connect(ui->lunchDays, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmDays);
    connect(ui->travelDays, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmDays);

    //修改补贴金额，不能超过标准
    connect(ui->lunchSubsidy, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);
    connect(ui->travelSubsidy, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);
    connect(ui->stayFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);

    //修改除了上3种补贴金额时，实时更新总金额
    connect(ui->cityTraFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::addAllFee);
    connect(ui->workFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::addAllFee);
    connect(ui->airportFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::addAllFee);
    connect(ui->otherFees, &QLineEdit::editingFinished, this, &insertPersonnelDialog::addAllFee);

    //切换货币，更改汇率
    connect(ui->comboBox1, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    connect(ui->comboBox2, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    connect(ui->comboBox3, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    connect(ui->comboBox4, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    connect(ui->comboBox5, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    connect(ui->comboBox6, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    connect(ui->comboBox7, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    connect(ui->comboBox8, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);

    connect(ui->waibi1, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    connect(ui->waibi2, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    connect(ui->waibi3, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    connect(ui->waibi4, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    connect(ui->waibi5, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    connect(ui->waibi6, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    connect(ui->waibi7, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    connect(ui->waibi8, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
}

void insertPersonnelDialog::disconnectItems()
{
    //选择是否为院内人员
    disconnect(groupBtn, SIGNAL(buttonClicked(int)), this, SLOT(isSchool(int)));

    //修改日期的时候，自动更新出差天数和补贴天数
    disconnect(ui->leaveDateEdit, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
    disconnect(ui->backDateEdit, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
    disconnect(ui->leaveDate, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
    disconnect(ui->backDate, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);

    //修改补贴天数的时候，要确认天数是否合理
    disconnect(ui->lunchDays, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmDays);
    disconnect(ui->travelDays, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmDays);

    //修改补贴金额，不能超过标准
    disconnect(ui->lunchSubsidy, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);
    disconnect(ui->travelSubsidy, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);
    disconnect(ui->stayFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);

    //修改除了上3种补贴金额时，实时更新总金额
    disconnect(ui->cityTraFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::addAllFee);
    disconnect(ui->workFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::addAllFee);
    disconnect(ui->airportFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::addAllFee);
    disconnect(ui->otherFees, &QLineEdit::editingFinished, this, &insertPersonnelDialog::addAllFee);

    //切换货币，更改汇率
    disconnect(ui->comboBox1, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    disconnect(ui->comboBox2, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    disconnect(ui->comboBox3, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    disconnect(ui->comboBox4, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    disconnect(ui->comboBox5, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    disconnect(ui->comboBox6, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    disconnect(ui->comboBox7, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);
    disconnect(ui->comboBox8, &QComboBox::currentTextChanged, this, &insertPersonnelDialog::getCoinType);

    disconnect(ui->waibi1, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    disconnect(ui->waibi2, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    disconnect(ui->waibi3, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    disconnect(ui->waibi4, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    disconnect(ui->waibi5, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    disconnect(ui->waibi6, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    disconnect(ui->waibi7, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
    disconnect(ui->waibi8, &QLineEdit::editingFinished, this, &insertPersonnelDialog::setValue);
}

void insertPersonnelDialog::setType(int type)
{
    personType = type;
    if (personType == 1)
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(1);
}

void insertPersonnelDialog::setTravelPerson(traBusPersonInfo *info)
{
    disconnectItems();

    traPerson = info;

    personType = 1;
    ui->stackedWidget->setCurrentIndex(0);

    ui->personName->setText(info->getStaffName());
    ui->personNumber->setText(QString::number(info->getStaffNumber()));
    ui->departmentEdit->setText(info->getDepartment());

    isInSchool = info->getIsInSchool();
    if (isInSchool) {
        ui->yesRadioBtn->setChecked(true);
        ui->searchPerson->show();
        ui->label_24->show();
        ui->personNumber->show();
        ui->label_25->show();
        ui->departmentEdit->show();
    } else {
        ui->noRadioBtn->setChecked(true);
        ui->searchPerson->hide();
        ui->label_24->hide();
        ui->personNumber->hide();
        ui->label_25->hide();
        ui->departmentEdit->hide();
    }

    ui->leaveDateEdit->setDate(info->getLeaveDate1());
    ui->backDateEdit->setDate(info->getReturnDate1());
    ui->busiDays->setText(QString::number(info->getDays()));

    ui->leavePlace->setText(info->getLeavePlace());
    ui->arrivaPlace->setText(info->getArrivePlace());
    ui->budgetNumEdit->setText(QString::number(info->getBudgetNumber()));
    ui->budgetNameEdit->setText(info->getBudgetName());
    ui->fundsTypeEdit->setText(info->getBudgetType());

    lunchPerDay = info->getLunchPerDay();
    travelPerDay = info->getTravelPerDay();
    stayPerDay = info->getStayPerDay();

    ui->cityTraFee->setText(QString::number(info->getCityFee()));
    ui->stayFee->setText(QString::number(info->getStayFee()));
    ui->workFee->setText(QString::number(info->getWorkFee()));
    ui->lunchDays->setText(QString::number(info->getLunchDays()));
    ui->lunchSubsidy->setText(QString::number(info->getLunchSubsidy()));
    ui->airportFee->setText(QString::number(info->getAirportFee()));
    ui->travelDays->setText(QString::number(info->getTraDays()));
    ui->travelSubsidy->setText(QString::number(info->getTraSubsidy()));
    ui->otherFees->setText(QString::number(info->getOtherFee()));

    connectItems();
}

void insertPersonnelDialog::setAbroadPerson(abroadPersonInfo *info)
{
    disconnectItems();

    abroadPerson = info;

    personType = 2;
    ui->stackedWidget->setCurrentIndex(1);

    ui->staffName->setText(info->getStaffName());
    ui->staffNumber->setText(QString::number(info->getStaffNumber()));
    ui->department->setText(info->getDepartment());

    ui->leaveDate->setDate(info->getLeaveDate1());
    ui->backDate->setDate(info->getReturnDate1());
    qint64 days = (info->getLeaveDate1()).daysTo(info->getReturnDate1());
    ui->days->setText(QString::number(days));

    ui->leaveCity->setText(info->getLeaveCity());
    ui->arriveCity->setText(info->getArriveCity());

    ui->budgetNum->setText(QString::number(info->getBudgetNum()));
    ui->budgetName->setText(info->getBudgetName());
    ui->budgetTypeEdit->setText(info->getBudgetType());

    connectItems();
}

void insertPersonnelDialog::isSchool(int buttonID)
{
    if (buttonID == 0){
        isInSchool = true;

        ui->searchPerson->show();
        ui->label_24->show();
        ui->personNumber->show();
        ui->label_25->show();
        ui->departmentEdit->show();
    }
    else if (buttonID == 1) {
        isInSchool = false;

        ui->searchPerson->hide();
        ui->label_24->hide();
        ui->personNumber->hide();
        ui->label_25->hide();
        ui->departmentEdit->hide();
    }
}

void insertPersonnelDialog::getDays()
{
    QDate date1, date2;
    qint64 days;
    //出差人员
    if (personType == 1) {
        date1 = ui->leaveDateEdit->date();
        date2 = ui->backDateEdit->date();
        days = date1.daysTo(date2);

        if (days < 0) {
            QMessageBox::warning(this, "waring", "启程时间不能比返回时间后", QMessageBox::Ok);
            disconnect(ui->leaveDateEdit, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
            ui->leaveDateEdit->setDate(date2);
            mDays = 0;
            ui->busiDays->setText("0");
            ui->lunchDays->setText("0");
            ui->travelDays->setText("0");
            connect(ui->leaveDateEdit, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
        } else {
            mDays = days;
            ui->busiDays->setText(QString::number(days));
            ui->lunchDays->setText(QString::number(days));
            ui->travelDays->setText(QString::number(days));

            travelSub = mDays * travelPerDay;
            lunchSub = mDays * lunchPerDay;
            staySub = (mDays - 1) * stayPerDay;

            ui->travelSubsidy->setText(QString::number(travelSub));
            ui->lunchSubsidy->setText(QString::number(lunchSub));
            ui->stayFee->setText(QString::number(staySub));
        }
    } else {    //出国人员
        date1 = ui->leaveDate->date();
        date2 = ui->backDate->date();

        days = date1.daysTo(date2);
        if(days < 0) {
            QMessageBox::warning(this, "waring", "启程时间不能比返回时间后", QMessageBox::Ok);
            disconnect(ui->leaveDate, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
            ui->leaveDate->setDate(date2);
            ui->days->setText("0");
            connect(ui->leaveDate, &QDateEdit::dateChanged, this, &insertPersonnelDialog::getDays);
        } else {
            mDays = days;
            ui->days->setText(QString::number(days));
        }
    }
}

void insertPersonnelDialog::confirmDays()
{
    int lunchDays = ui->lunchDays->text().toInt();          //伙食补贴天数
    int traDays = ui->travelDays->text().toInt();           //交通补贴天数

    disconnect(ui->lunchDays, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmDays);
    disconnect(ui->travelDays, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmDays);

    if (lunchDays > mDays) {
        QMessageBox::warning(this, "waring", "伙食补贴天数不得多于出差天数", QMessageBox::Ok);
        ui->lunchDays->setText(QString::number(mDays));
    }
    if (traDays > mDays) {
        QMessageBox::warning(this, "waring", "交通补贴天数不得多于出差天数", QMessageBox::Ok);
        ui->travelDays->setText(QString::number(mDays));
    }

    lunchSub = lunchDays * lunchPerDay;
    travelSub = traDays * travelPerDay;

    ui->lunchSubsidy->setText(QString::number(lunchSub));
    ui->travelSubsidy->setText(QString::number(travelSub));

    connect(ui->lunchDays, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmDays);
    connect(ui->travelDays, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmDays);

    addAllFee();
}

void insertPersonnelDialog::addAllFee()
{
    int cityTraFee = ui->cityTraFee->text().toInt();
    int stayFee = ui->stayFee->text().toInt();
    int workFee = ui->workFee->text().toInt();
    int lunchSubsidy = ui->lunchSubsidy->text().toInt();
    int airportFee = ui->airportFee->text().toInt();
    int travelSubsidy = ui->travelSubsidy->text().toInt();
    int otherFees = ui->otherFees->text().toInt();

    totalFee = cityTraFee + stayFee + workFee + lunchSubsidy + airportFee + travelSubsidy + otherFees;

    ui->totalFee->setNum(totalFee);
}

void insertPersonnelDialog::confirmSub()
{
    int lunchSubs = ui->lunchSubsidy->text().toInt();
    int traverSubs = ui->travelSubsidy->text().toInt();
    int staySubs = ui->stayFee->text().toInt();

    disconnect(ui->lunchSubsidy, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);
    disconnect(ui->travelSubsidy, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);
    disconnect(ui->stayFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);

    if(lunchSubs > lunchSub) {
        QMessageBox::warning(this, "warning", "伙食补贴不得超过标准", QMessageBox::Ok);
        ui->lunchSubsidy->setText(QString::number(lunchSub));
    }
    if(traverSubs > travelSub) {
        QMessageBox::warning(this, "warning", "伙食补贴不得超过标准", QMessageBox::Ok);
        ui->travelSubsidy->setText(QString::number(travelSub));
    }
    if(staySubs > staySub) {
        QMessageBox::warning(this, "warning", "伙食补贴不得超过标准", QMessageBox::Ok);
        ui->stayFee->setText(QString::number(staySub));
    }
    connect(ui->lunchSubsidy, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);
    connect(ui->travelSubsidy, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);
    connect(ui->stayFee, &QLineEdit::editingFinished, this, &insertPersonnelDialog::confirmSub);

    addAllFee();
}

//设置汇率
void insertPersonnelDialog::getCoinType()
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

double insertPersonnelDialog::chooseCoinType(QString &str)
{
    if ("美元" == str)
        return 7.14;
    else if("英镑" == str)
        return 8.74;
    return 0;
}

void insertPersonnelDialog::setValue()
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

    ui->totalFee_2->setNum(total);
}

void insertPersonnelDialog::on_saveBtn_clicked()
{
    bool finished = true;
    QString str = "请填写：";
    if (personType == 1)        //差旅报销人员添加
    {
        if (isInSchool) {
            if (ui->personName->text().isEmpty()) {
                finished = false;
                str = QString("%1 %2 ").arg(str).arg("出差人员");
            }
//            if (ui->personNumber->text().isEmpty()) {
//                finished = false;
//                str = QString("%1 %2 ").arg(str).arg("员工工号");
//            }
            if (ui->departmentEdit->text().isEmpty()) {
                finished = false;
                str = QString("%1 %2 ").arg(str).arg("部门");
            }
        }
        if (ui->leavePlace->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("出发地点");
        }
        if (ui->arrivaPlace->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("到达地点");
        }
        if (ui->budgetNumEdit->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("预算项目号");
        }
        if (ui->budgetNameEdit->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("预算项目名");
        }
        if (ui->fundsTypeEdit->text().isEmpty() ){
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("经费类型");
        }

        if (!finished)
            QMessageBox::warning(this, "warning", str, QMessageBox::Ok);
        else {
            //保存差旅人员信息
            traPerson->setStaffName(ui->personName->text());
            traPerson->setStaffNumber(ui->personNumber->text().toInt());
            traPerson->setIsInSchool(isInSchool);
            traPerson->setLeaveDate(ui->leaveDateEdit->text());
            traPerson->setLeaveDate1(ui->leaveDateEdit->date());
            traPerson->setReturnDate(ui->backDateEdit->text());
            traPerson->setReturnDate1(ui->backDateEdit->date());
            traPerson->setDays(ui->busiDays->text().toInt());
            traPerson->setDepartment(ui->departmentEdit->text());
            traPerson->setLeavePlace(ui->leavePlace->text());
            traPerson->setArrivePlace(ui->arrivaPlace->text());

            traPerson->setBudgetNumber(ui->budgetNumEdit->text().toInt());
            traPerson->setBudgetName(ui->budgetNameEdit->text());
            traPerson->setBudgetType(ui->fundsTypeEdit->text());

            //保存每日标准
            traPerson->setLunchPerDay(lunchPerDay);
            traPerson->setTravelPerDay(travelPerDay);
            traPerson->setStayPerDay(stayPerDay);

            int     cityFee = ui->cityTraFee->text().toInt();           //城市间交通费
            int     stayFee = ui->stayFee->text().toInt();              //住宿费
            int     workFee = ui->workFee->text().toInt();              //会务费/注册费
            int     lunchDays = ui->lunchDays->text().toInt();          //伙食补贴天数
            int     lunchSubsidy = ui->lunchSubsidy->text().toInt();    //伙食补贴费
            int     airportFee = ui->airportFee->text().toInt();        //往返机场交通费
            int     traDays = ui->travelDays->text().toInt();           //交通补贴天数
            int     traSubsidy = ui->travelSubsidy->text().toInt();     //交通补贴费
            int     otherFee = ui->otherFees->text().toInt();           //其他费用

            int totalFee = cityFee + stayFee + workFee + lunchSubsidy + airportFee + traSubsidy + otherFee;

            traPerson->setCityFee(cityFee);
            traPerson->setStayFee(stayFee);
            traPerson->setWorkFee(workFee);
            traPerson->setLunchDays(lunchDays);
            traPerson->setLunchSubsidy(lunchSubsidy);
            traPerson->setAirportFee(airportFee);
            traPerson->setTraDays(traDays);
            traPerson->setTraSubsidy(traSubsidy);
            traPerson->setOtherFee(otherFee);

            traPerson->setTotalFee(totalFee);

            emit addTravelPerson(mIndex, traPerson);

            close();
        }
    }
    else                        //出国报销人员添加
    {
        if (ui->staffName->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("员工姓名");
        }
//        if (ui->staffNumber->text().isEmpty()) {
//            finished = false;
//            str = QString("%1 %2 ").arg(str).arg("员工工号");
//        }
        if (ui->department->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("部门");
        }
        if (ui->leaveCity->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("出发城市");
        }
        if (ui->arriveCity->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("到达城市");
        }
        if (ui->budgetNumEdit->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("项目经费号");
        }
        if (ui->budgetNameEdit->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("项目名");
        }

        if (!finished)
            QMessageBox::warning(this, "warning", str, QMessageBox::Ok);
        else {
            //保存信息
            abroadPerson->setStaffName(ui->staffName->text());
            abroadPerson->setStaffNumber(ui->staffNumber->text().toInt());
            abroadPerson->setDepartment(ui->department->text());
            abroadPerson->setLeaveDate(ui->leaveDateEdit->text());
            abroadPerson->setLeaveDate1(ui->leaveDateEdit->date());
            abroadPerson->setReturnDate(ui->backDateEdit->text());
            abroadPerson->setReturnDate1(ui->backDateEdit->date());

            QDate date1 = ui->leaveDate->date();
            QDate date2 = ui->backDate->date();
            qint64 days = date1.daysTo(date2);
            abroadPerson->setDays(days);

            abroadPerson->setLeaveCity(ui->leaveCity->text());
            abroadPerson->setArriveCity(ui->arriveCity->text());
            abroadPerson->setBudgetNum(ui->budgetNumEdit->text().toInt());
            abroadPerson->setBudgetName(ui->budgetNameEdit->text());
            abroadPerson->setBudgetType(ui->budgetTypeEdit->text());

            double rmb1 = ui->rmb1->text().toDouble();
            double rmb2 = ui->rmb1->text().toDouble();
            double rmb3 = ui->rmb1->text().toDouble();
            double rmb4 = ui->rmb1->text().toDouble();
            double rmb5 = ui->rmb1->text().toDouble();
            double rmb6 = ui->rmb1->text().toDouble();
            double rmb7 = ui->rmb1->text().toDouble();
            double rmb8 = ui->rmb1->text().toDouble();

            double total = rmb1 + rmb2 + rmb3 + rmb4 + rmb5 + rmb6 + rmb7 + rmb8;
            abroadPerson->setTotalFee(total);

            emit addAbroadPerson(mIndex, abroadPerson);

            close();
        }
    }
}

void insertPersonnelDialog::on_exitBtn_clicked()
{
    if (mIndex > -1) {
        if (personType == 1)
            emit addTravelPerson(mIndex, traPerson);
        if (personType == 2)
            emit addAbroadPerson(mIndex, abroadPerson);
    }
    close();
}


void insertPersonnelDialog::on_searchPerson_clicked()
{
    allInterface::getinstance ()->info.setname (ui->personName->text ());
    allInterface::getinstance ()->getuserdatalist ();
    connect (allInterface::getinstance (),SIGNAL(setUserDataListDone()),this,SLOT(dealUserList()));
}

void insertPersonnelDialog::dealUserList()
{
    if (personType == 1)
        ui->departmentEdit->setText(allInterface::getinstance()->info.getofficeName());
    else
        ui->department->setText(allInterface::getinstance()->info.getofficeName());
}

void insertPersonnelDialog::on_arrivaPlace_editingFinished()
{
    QString arrivaPlace = ui->arrivaPlace->text ();
    qDebug()<<"到达城市"<<arrivaPlace;
    allInterface::getinstance ()->info.setarriveCity (arrivaPlace);
    allInterface::getinstance ()->getdataTravel ();
    connect (allInterface::getinstance (),SIGNAL(setDataTravelDone()),this,SLOT(dealDataTravel()));
}

void insertPersonnelDialog::dealDataTravel()
{
//    qDebug()<<"住宿标准"<<allInterface::getinstance ()->getinfo().getstaySubsidy ();
//    qDebug()<<"吃饭标准"<<allInterface::getinstance ()->getinfo().getmealSubsidy ();
//    qDebug()<<"交通标准"<<allInterface::getinstance ()->getinfo().gettrafficSubsidy ();

    lunchPerDay = allInterface::getinstance ()->getinfo().getmealSubsidy();     //吃饭标准
    travelPerDay = allInterface::getinstance ()->getinfo().gettrafficSubsidy(); //交通标准
    stayPerDay = allInterface::getinstance ()->getinfo().getstaySubsidy();      //住宿标准
}

void insertPersonnelDialog::on_staffName_editingFinished()
{
    QString staffName = ui->staffName->text();
    allInterface::getinstance()->info.setname(staffName);
    allInterface::getinstance()->getuserdatalist();
    connect(allInterface::getinstance(), SIGNAL(setUserDataListDone()),this, SLOT(dealUserList()));
}

void insertPersonnelDialog::on_arriveCity_editingFinished()
{
    allInterface::getinstance ()->getinfo ().setarriveCity (ui->arriveCity->text ());
    allInterface::getinstance ()->getdataCountriesExpense ();
    connect (allInterface::getinstance (),SIGNAL(setdataCountriesExpenseDone()),this,SLOT(dealdataCountriesExpense()));
}

void insertPersonnelDialog::dealdataCountriesExpense()
{
//    qDebug()<<"注册费"<<allInterface::getinstance ()->getinfo ().getaboradStaySubsidy ();
//    qDebug()<<"公杂费"<<allInterface::getinstance ()->getinfo ().getcommonMiscellaneous ();
//    qDebug()<<"伙食费"<<allInterface::getinstance ()->getinfo ().getaboradMealSubsidy ();
//    qDebug()<<"币种"<<allInterface::getinstance ()->getinfo ().getcurrencyName ();

    aboradStaySubsidy = allInterface::getinstance ()->getinfo ().getaboradStaySubsidy ();
    commonMiscellaneous = allInterface::getinstance ()->getinfo ().getcommonMiscellaneous ();
    aboradMealSubsidy = allInterface::getinstance ()->getinfo ().getaboradMealSubsidy ();
    currencyName = allInterface::getinstance ()->getinfo ().getcurrencyName ();

}

void insertPersonnelDialog::on_searchBudgetNum_clicked()
{

}
