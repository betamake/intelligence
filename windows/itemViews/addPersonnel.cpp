#include "addPersonnel.h"
#include "ui_addPersonnel.h"
#include "managers/personnelmanager.h"
#include <QMessageBox>

addPersonnel::addPersonnel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addPersonnel)
{
    ui->setupUi(this);

    traPerInfo = new traBusPersonInfo();

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
    if (ui->lineEdit->text().isEmpty() || ui->busiPerson->text().isEmpty() || ui->staffNumber->text().isEmpty() ||
           ui->leaveDate->text().isEmpty() || ui->returnDate->text().isEmpty() || ui->departmentEdit->text().isEmpty() ||
           ui->busiDays->text().isEmpty() || ui->leavePlace->text().isEmpty() || ui->arrivaPlace->text().isEmpty() ||
           ui->budgetNumEdit->text().isEmpty() || ui->budgetNameEdit->text().isEmpty() || ui->fundsTypeEdit->text().isEmpty() ){
        QMessageBox::warning(this, "warning", "还有信息未输入", QMessageBox::Ok);
    } else {
        //保存差旅人员信息
        traPerInfo->setStaffName(ui->busiPerson->text());
        traPerInfo->setStaffNumber(ui->staffNumber->text().toInt());
        traPerInfo->setLeaveDate(ui->leaveDate->text());
        traPerInfo->setReturnDate(ui->returnDate->text());
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
