#include "addPersonnel.h"
#include "ui_addPersonnel.h"
#include "managers/personnelmanager.h"
#include <QMessageBox>
#include <QDebug>
#include <QDate>

addPersonnel::addPersonnel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addPersonnel)
{
    ui->setupUi(this);

    initItemView();

    mIndex = -1;
    traPerInfo = new traBusPersonInfo();
    businessdays = 0;           //出差天数

    lunchSub = 0;
    travelSub = 0;
    staySub = 0;

    //每日补贴先设定固定值，之后获得
    lunchPerDay = 100;      //伙食补贴按每天100来算
    travelPerDay = 50;      //交通补贴按每天50来算
    stayPerday = 300;       //住宿补贴按每天300来算

    //选择是否为院内人员
    connect(ui->yesRadioBtn, SIGNAL(clicked(bool)), this, SLOT(isSchool));
    connect(ui->noRadioBtn, SIGNAL(clicked(bool)), this, SLOT(isSchool));

    //修改日期的时候，自动更新出差天数和补贴天数
    connect(ui->leaveDateEdit, &QDateEdit::dateChanged, this, &addPersonnel::getDays);
    connect(ui->backDateEdit, &QDateEdit::dateChanged, this, &addPersonnel::getDays);

    //修改补贴天数的时候，要确认天数是否合理
    connect(ui->lunchDays, &QLineEdit::editingFinished, this, &addPersonnel::confirmDays);
    connect(ui->travelDays, &QLineEdit::editingFinished, this, &addPersonnel::confirmDays);

    //修改补贴金额，不能超过标准
    connect(ui->lunchSubsidy, &QLineEdit::editingFinished, this, &addPersonnel::confirmSub);
    connect(ui->travelSubsidy, &QLineEdit::editingFinished, this, &addPersonnel::confirmSub);
    connect(ui->stayFee, &QLineEdit::editingFinished, this, &addPersonnel::confirmSub);

    //修改除了上3种补贴金额时，实时更新总金额
    connect(ui->cityTraFee, &QLineEdit::editingFinished, this, &addPersonnel::addAllFee);
    connect(ui->workFee, &QLineEdit::editingFinished, this, &addPersonnel::addAllFee);
    connect(ui->airportFee, &QLineEdit::editingFinished, this, &addPersonnel::addAllFee);
    connect(ui->otherFees, &QLineEdit::editingFinished, this, &addPersonnel::addAllFee);
}

addPersonnel::~addPersonnel()
{
    delete ui;
}

//初始化时的一些控件上的配置，信号和槽connect写在构造函数中
void addPersonnel::initItemView()
{
    //是否社内人员选择
    groupBtn = new QButtonGroup(this);
    groupBtn->addButton(ui->yesRadioBtn, 0);
    groupBtn->addButton(ui->noRadioBtn, 1);
    ui->yesRadioBtn->setChecked(true);          //默认是社内人员
    isInSchool = true;

    //日期选择
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
}

void addPersonnel::setIndex(int index)
{
    mIndex = index;
    ui->indexLab1->setNum(index);
    ui->indexLab2->setNum(index);
}

void addPersonnel::on_cleraBtn_clicked()
{
    ui->budgetNumEdit->clear();
    ui->budgetNameEdit->clear();
    ui->fundsTypeEdit->clear();
    ui->cityTraFee->clear();
    ui->stayFee->clear();
    ui->workFee->clear();
    ui->lunchDays->clear();
    ui->lunchSubsidy->clear();
    ui->airportFee->clear();
    ui->travelDays->clear();
    ui->travelSubsidy->clear();
    ui->otherFees->clear();
}

void addPersonnel::getDays()
{
    QDate date1 = ui->leaveDateEdit->date();
    QDate date2 = ui->backDateEdit->date();

    qint64 days = date1.daysTo(date2);
    if(days < 0) {
        QMessageBox::warning(this, "waring", "启程时间不能比返回时间后", QMessageBox::Ok);
        businessdays = 0;
        ui->busiDays->setText("0");
        ui->lunchDays->setText("0");
        ui->travelDays->setText("0");

        addAllFee();
    } else {
        qDebug() << "days大于或等于0" << endl;
        businessdays = days;
        ui->busiDays->setText(QString::number(days));
        ui->lunchDays->setText(QString::number(days));      //伙食补贴天数先设置为出差天数
        ui->travelDays->setText(QString::number(days));     //交通补贴天数先设置为出差天数

        travelSub = businessdays * travelPerDay;
        lunchSub = businessdays * lunchPerDay;
        staySub = (businessdays-1) * stayPerday;

        ui->travelSubsidy->setText(QString::number(travelSub));
        ui->lunchSubsidy->setText(QString::number(lunchSub));
        ui->stayFee->setText(QString::number(staySub));

        addAllFee();
    }

}

void addPersonnel::confirmDays()
{
    int lunchDays = ui->lunchDays->text().toInt();          //伙食补贴天数
    int traDays = ui->travelDays->text().toInt();           //交通补贴天数

    disconnect(ui->lunchDays, &QLineEdit::editingFinished, this, &addPersonnel::confirmDays);
    disconnect(ui->travelDays, &QLineEdit::editingFinished, this, &addPersonnel::confirmDays);

    if (lunchDays > businessdays) {
        QMessageBox::warning(this, "waring", "伙食补贴天数不得多于出差天数", QMessageBox::Ok);
        ui->lunchDays->setText(QString::number(businessdays));
    }
    if (traDays > businessdays) {
        QMessageBox::warning(this, "waring", "交通补贴天数不得多于出差天数", QMessageBox::Ok);
        ui->travelDays->setText(QString::number(businessdays));
    }

    lunchSub = lunchDays * lunchPerDay;
    travelSub = traDays * travelPerDay;

    ui->lunchSubsidy->setText(QString::number(lunchSub));
    ui->travelSubsidy->setText(QString::number(travelSub));

    connect(ui->lunchDays, &QLineEdit::editingFinished, this, &addPersonnel::confirmDays);
    connect(ui->travelDays, &QLineEdit::editingFinished, this, &addPersonnel::confirmDays);

    addAllFee();
}

void addPersonnel::confirmSub()
{
    int lunchSubs = ui->lunchSubsidy->text().toInt();
    int traverSubs = ui->travelSubsidy->text().toInt();
    int staySubs = ui->stayFee->text().toInt();

    disconnect(ui->lunchSubsidy, &QLineEdit::editingFinished, this, &addPersonnel::confirmSub);
    disconnect(ui->travelSubsidy, &QLineEdit::editingFinished, this, &addPersonnel::confirmSub);
    disconnect(ui->stayFee, &QLineEdit::editingFinished, this, &addPersonnel::confirmSub);

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
    connect(ui->lunchSubsidy, &QLineEdit::editingFinished, this, &addPersonnel::confirmSub);
    connect(ui->travelSubsidy, &QLineEdit::editingFinished, this, &addPersonnel::confirmSub);
    connect(ui->stayFee, &QLineEdit::editingFinished, this, &addPersonnel::confirmSub);

    addAllFee();
}

void addPersonnel::addAllFee()
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

//设置是否院内人员，是的话必须输入员工号和编码等信息，不是的话就不用输
//同时，院内人员和院外人员的补贴是不一样的
void addPersonnel::isSchool()
{
    if (ui->yesRadioBtn->isChecked())
        isInSchool = true;
    if (ui->noRadioBtn->isChecked())
        isInSchool = false;
}

//保存人员的信息
void addPersonnel::saveItem()
{
    bool finished = true;
    QString str = "请填写：";

    //是院内人员的时候，必须输入人员名和人员工号
    if (isInSchool) {
        if (ui->busiPerson->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("出差人员");
//            QMessageBox::warning(this, "warning", "请输入出差人员", QMessageBox::Ok);
        }
        if (ui->staffNumber->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("员工工号");
        }
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

    if(!finished)
        QMessageBox::warning(this, "warning", str, QMessageBox::Ok);
    else {
        QString leaveDateStr = ui->leaveDateEdit->text();
        QString backDateStr = ui->backDateEdit->text();

        QDate date1 = ui->leaveDateEdit->date();
        QDate date2 = ui->backDateEdit->date();

        qint64 days1 = date2.daysTo(date1);
        qDebug() << "days:" << days1;

        //保存差旅人员信息
        traPerInfo->setStaffName(ui->busiPerson->text());
        traPerInfo->setStaffNumber(ui->staffNumber->text().toInt());
        traPerInfo->setLeaveDate(ui->leaveDateEdit->text());
        traPerInfo->setLeaveDate1(ui->leaveDateEdit->date());
        traPerInfo->setReturnDate(ui->backDateEdit->text());
        traPerInfo->setReturnDate1(ui->backDateEdit->date());
        traPerInfo->setDays(ui->busiDays->text().toInt());
        traPerInfo->setDepartment(ui->departmentEdit->text());
        traPerInfo->setLeavePlace(ui->leavePlace->text());
        traPerInfo->setArrivePlace(ui->arrivaPlace->text());

        traPerInfo->setBudgetNumber(ui->budgetNumEdit->text().toInt());
        traPerInfo->setBudgetName(ui->budgetNameEdit->text());
        traPerInfo->setBudgetType(ui->fundsTypeEdit->text());

        int     cityFee = ui->cityTraFee->text().toInt();            //城市间交通费
        int     stayFee = ui->stayFee->text().toInt();            //住宿费
        int     workFee = ui->workFee->text().toInt();            //会务费/注册费
        int     lunchDays = ui->lunchDays->text().toInt();          //伙食补贴天数
        int     lunchSubsidy = ui->lunchSubsidy->text().toInt();       //伙食补贴费
        int     airportFee = ui->airportFee->text().toInt();         //往返机场交通费
        int     traDays = ui->travelDays->text().toInt();            //交通补贴天数
        int     traSubsidy = ui->travelSubsidy->text().toInt();         //交通补贴费
        int     otherFee = ui->otherFees->text().toInt();           //其他费用

        int totalFee = cityFee + stayFee + workFee + lunchSubsidy + airportFee + traSubsidy + otherFee;

        traPerInfo->setCityFee(cityFee);
        traPerInfo->setStayFee(stayFee);
        traPerInfo->setWorkFee(workFee);
        traPerInfo->setLunchDays(lunchDays);
        traPerInfo->setLunchSubsidy(lunchSubsidy);
        traPerInfo->setAirportFee(airportFee);
        traPerInfo->setTraDays(traDays);
        traPerInfo->setTraSubsidy(traSubsidy);
        traPerInfo->setOtherFee(otherFee);

        traPerInfo->setTotalFee(totalFee);

        emit addToMainWindow(mIndex, traPerInfo);

//        //将人员信息结构体保存起来
//        personnelManager::getInstance()->addTravelItem(traPerInfo);

//        emit added();
    }
}

void addPersonnel::setPerson(traBusPersonInfo *info)
{
    ui->busiPerson->setText(info->getStaffName());
    ui->staffNumber->setText(QString::number(info->getStaffNumber()));
//    ui->leaveDateEdit->setDate(info->getLeaveDate1());
//    ui->backDateEdit->setDate(info->getReturnDate1());
//    ui->busiDays->setText(QString::number(info->getDays()));
    ui->departmentEdit->setText(info->getDepartment());
    ui->leavePlace->setText(info->getLeavePlace());
    ui->arrivaPlace->setText(info->getArrivePlace());
    ui->budgetNumEdit->setText(QString::number(info->getBudgetNumber()));
    ui->budgetNameEdit->setText(info->getBudgetName());
    ui->fundsTypeEdit->setText(info->getBudgetType());

}
