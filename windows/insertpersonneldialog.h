#ifndef INSERTPERSONNELDIALOG_H
#define INSERTPERSONNELDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <allinterface.h>
#include "managers/personnelmanager.h"

namespace Ui {
class insertPersonnelDialog;
}

class insertPersonnelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit insertPersonnelDialog( QWidget *parent = nullptr);
    ~insertPersonnelDialog();

    void setType(int type);

    void setIndex(int index) { mIndex = index;}

    void setTravelPerson(traBusPersonInfo *info);
    void setAbroadPerson(abroadPersonInfo *info);

private:
    void initItemView();

    double chooseCoinType(QString &str);

    void connectItems();            //把初始化时需要连接qDebug() <<的信号和槽单独取出来，方便操作
    void disconnectItems();         //打开编辑现有人员的时候，需要把构造函数里的信号和槽都断开，不然会带来不便


signals:
    void addTravelPerson(int index, traBusPersonInfo *info);          //添加差旅人员的信号
    void addAbroadPerson(int index, abroadPersonInfo *info);    //添加出国人员的信号

private slots:
    void on_saveBtn_clicked();
    void on_exitBtn_clicked();

    //出差人员特有的槽函数
    void isSchool(int buttonID);
    void confirmDays();         //天数限制
    void confirmSub();          //补贴不能超过标准
    void addAllFee();           //计算所有的费用
    void dealDataTravel();      //处理差旅标准信息

    //出国人员特有的槽函数
    void getCoinType();         //选择汇率
    void setValue();            //计算人民币金额
    void dealdataCountriesExpense();    //出国城市费用

    //两个页面共有的槽函数
    void getDays();             //计算天数

    void dealUserList();        //处理返回的user信息


    void on_searchPerson_clicked();

    void on_arrivaPlace_editingFinished();

    void on_staffName_editingFinished();

    void on_arriveCity_editingFinished();

    void on_searchBudgetNum_clicked();

private:
    Ui::insertPersonnelDialog *ui;

    int personType;             //人员类型，1为出差，2为出国

    int mIndex;                  //在ListWidget和List中的位置，用于修改信息时插入

    QButtonGroup *groupBtn;
    bool isInSchool;            //是否院内人员

    traBusPersonInfo* traPerson;
    abroadPersonInfo* abroadPerson;

    int mDays;                  //出差或出国天数
    int totalFee;               //总费用

    //每日补贴，从接口获得
    int lunchPerDay;
    int travelPerDay;
    int stayPerDay;
    //补贴，每日补贴乘天数
    int lunchSub;
    int travelSub;
    int staySub;

    //出国人员自动带出
    int aboradStaySubsidy; //国外注册费用
    int commonMiscellaneous;//公杂费
    int aboradMealSubsidy;//国外伙食标准
    QString currencyName;//币种



};

#endif // INSERTPERSONNELDIALOG_H
