#include "insertpersonneldialog.h"
#include "ui_insertpersonneldialog.h"

#include <QDebug>

insertPersonnelDialog::insertPersonnelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::insertPersonnelDialog)
{
    personType = 1;
    ui->stackedWidget->setCurrentIndex(0);

    ui->setupUi(this);
}

insertPersonnelDialog::~insertPersonnelDialog()
{
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


void insertPersonnelDialog::setType(int type)
{
    personType = type;
    if (personType == 1)
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(1);

}

void insertPersonnelDialog::isSchool()
{

}


void insertPersonnelDialog::on_saveBtn_clicked()
{
    if (personType == 2)        //出国报销人员添加
    {
        emit addAllAbroadPerson();
    }
    else                        //差旅报销人员添加
    {
        emit addAllPerson();
    }
}

void insertPersonnelDialog::on_exitBtn_clicked()
{
    close();
}

void insertPersonnelDialog::addPerCount()
{

}
