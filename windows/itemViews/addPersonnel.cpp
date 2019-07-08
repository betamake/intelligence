#include "addPersonnel.h"
#include "ui_addPersonnel.h"

addPersonnel::addPersonnel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addPersonnel)
{
    ui->setupUi(this);

    setNemberOnly();
}

addPersonnel::~addPersonnel()
{
    delete ui;
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

//设置lineEdit只能输入数字，以免出现问题，目前只能输入整数，小数等之后完善
void addPersonnel::setNemberOnly()
{
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

//保存人员的信息
void addPersonnel::saveItem()
{

}
