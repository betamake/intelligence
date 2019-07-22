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

    traPerInfo = new traBusPersonInfo();
    businessdays = 0;           //出差天数
    lunchPerDay = 0;
    lunchSub = 0;
    travelPerDay = 0;
    travelSub = 0;

    //选择是否为院内人员
    connect(ui->yesRadioBtn, SIGNAL(clicked(bool)), this, SLOT(isSchool));
    connect(ui->noRadioBtn, SIGNAL(clicked(bool)), this, SLOT(isSchool));

    //修改日期的时候，自动更新出差天数和补贴天数
    connect(ui->leaveDateEdit, &QDateEdit::dateChanged, this, &addPersonnel::getDays);
    connect(ui->backDateEdit, &QDateEdit::dateChanged, this, &addPersonnel::getDays);

    //修改补贴天数的时候，要确认天数是否合理
    connect(ui->lunchDays, &QLineEdit::editingFinished, this, &addPersonnel::confirmDays);
    connect(ui->travelDays, &QLineEdit::editingFinished, this, &addPersonnel::confirmDays);

}

addPersonnel::~addPersonnel()
{
    delete ui;
}

//初始化时的一些控件上的配置，信号和槽connect写在构造函数中
void addPersonnel::initItemView()
{
    //是否社内人员选择
    groupBtn1 = new QButtonGroup(this);
    groupBtn1->addButton(ui->yesRadioBtn, 0);
    groupBtn1->addButton(ui->noRadioBtn, 1);
    ui->yesRadioBtn->setChecked(true);          //默认是社内人员
    isInSchool = true;

    //日期选择
    //首先设置下拉选择日期
    ui->leaveDateEdit->setCalendarPopup(true);
    ui->backDateEdit->setCalendarPopup(true);
    //给日期框分配当天的日期
    QDate date = QDate::currentDate();
    ui->leaveDateEdit->setDate(date);
    ui->backDateEdit->setDate(date);
    //出差天数，补贴天数初始化为0
    ui->busiDays->setText("0");
    ui->lunchDays->setText("0");
    ui->travelDays->setText("0");


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
    } else {
        qDebug() << "days大于或等于0" << endl;
        businessdays = days;
        ui->busiDays->setText(QString::number(days));
        ui->lunchDays->setText(QString::number(days));      //伙食补贴天数
        ui->travelDays->setText(QString::number(days));     //交通补贴天数
    }
}

void addPersonnel::confirmDays()
{
    int lunchDays = ui->lunchDays->text().toInt();          //伙食补贴天数
    int traDays = ui->travelDays->text().toInt();           //交通补贴天数

    if (lunchDays > businessdays) {
        QMessageBox::warning(this, "waring", "伙食补贴天数不得多于出差天数", QMessageBox::Ok);
        ui->lunchDays->setText(QString::number(businessdays));
    }
    if (traDays > businessdays) {
        QMessageBox::warning(this, "waring", "交通补贴天数不得多于出差天数", QMessageBox::Ok);
        ui->travelDays->setText(QString::number(businessdays));
    }

    //每日补贴先设定固定值，之后获得
    lunchPerDay = 100;      //伙食补贴按每天100来算
    travelPerDay = 50;      //交通补贴按每天50来算
    lunchSub = lunchDays * lunchPerDay;
    travelSub = traDays * travelPerDay;

}

//设置是否院内人员，是的话必须输入员工号和编码等信息，不是的话就不用输
//同时，院内人员和院外人员的补贴是不一样的
void addPersonnel::isSchool()
{

}

//保存人员的信息
void addPersonnel::saveItem()
{
    if (ui->busiPerson->text().isEmpty() || ui->staffNumber->text().isEmpty() ||
           ui->departmentEdit->text().isEmpty() ||
           ui->busiDays->text().isEmpty() || ui->leavePlace->text().isEmpty() || ui->arrivaPlace->text().isEmpty() ||
           ui->budgetNumEdit->text().isEmpty() || ui->budgetNameEdit->text().isEmpty() || ui->fundsTypeEdit->text().isEmpty() ){
        QMessageBox::warning(this, "warning", "还有信息未输入", QMessageBox::Ok);
    } else {
        QString leaveDateStr = ui->leaveDateEdit->text();
        QString backDateStr = ui->backDateEdit->text();

        QDate date1 = ui->leaveDateEdit->date();
        QDate date2 = ui->backDateEdit->date();

        qint64 days1 = date2.daysTo(date1);
        qDebug() << "days:" << days1;

        //保存差旅人员信息
        traPerInfo->setStaffName(ui->busiPerson->text());
        traPerInfo->setStaffNumber(ui->staffNumber->text().toInt());
//        traPerInfo->setLeaveDate(ui->leaveDate->text());
//        traPerInfo->setReturnDate(ui->returnDate->text());
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


        //将人员信息结构体保存起来
        personnelManager::getInstance()->addTravelItem(traPerInfo);

        emit added();
    }
}
