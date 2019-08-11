#ifndef ADDPERSONNEL_H
#define ADDPERSONNEL_H

#include <QWidget>
#include <QButtonGroup>
#include "managers/personnelmanager.h"
#include <allinterface.h>

//差旅报销人员信息录入

namespace Ui {
class addPersonnel;
}
class traBusPersonInfo;

class addPersonnel : public QWidget
{
    Q_OBJECT

public:
    explicit addPersonnel(QWidget *parent = nullptr);
    ~addPersonnel();

    void setIndex(int index);

    void setPerson(traBusPersonInfo *info);

    traBusPersonInfo *getPerson() const { return traPerInfo;}

private:
    void initItemView();    //初始化页面显示


signals:
    void added();

    void addToMainWindow(int index, traBusPersonInfo *info);

private slots:
    void on_cleraBtn_clicked();
    void isSchool();        //是否院内人员
    void getDays();         //计算出差天数，填入天数，补贴天数的输入框中
    void confirmDays();     //修改补贴天数的时候要保证天数不得多于出差天数
    void confirmSub();      //修改补贴金额不能超过标准x天数
    void addAllFee();       //添加所有金额

    void on_searchPerson_clicked();

    void on_arrivaPlace_editingFinished();

public slots:
    void saveItem();
    void dealUserList();//处理返回的user信息;
    void dealDataTravel();//处理差旅标准信息;

private:
    Ui::addPersonnel *ui;

    traBusPersonInfo *traPerInfo;

    int mIndex;

    bool isInSchool;        //是否院内人员
    int businessdays;       //通过选日期控件获得的天数

    int lunchPerDay;        //伙食每日补贴
    int lunchSub;           //伙食补贴，由天数x每日补贴自动获得
    int travelPerDay;       //交通每日补贴
    int travelSub;          //交通补贴，由天数x每日补贴自动获得
    int stayPerday;         //住宿每日补贴
    int staySub;            //住宿补贴，由(天数-1)x每日补贴自动获得

    int totalFee;           //总费用

    QButtonGroup *groupBtn;
};

#endif // ADDPERSONNEL_H
