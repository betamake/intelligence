#ifndef ADDPERSONNEL_H
#define ADDPERSONNEL_H

#include <QWidget>
#include <QButtonGroup>
#include "managers/personnelmanager.h"

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

private:
    void initItemView();    //初始化页面显示

signals:
    void added();

private slots:
    void on_cleraBtn_clicked();
    void isSchool();        //是否院内人员
    void getDays();         //计算出差天数，填入天数，补贴天数的输入框中
    void confirmDays();     //修改补贴天数的时候要保证天数不得多于出差天数


public slots:
    void saveItem();

private:
    Ui::addPersonnel *ui;

    traBusPersonInfo *traPerInfo;

    bool isInSchool;        //是否院内人员
    int businessdays;       //通过选日期控件获得的天数

    int lunchPerDay;        //伙食每日补贴
    int lunchSub;           //伙食补贴，由天数x每日补贴自动获得
    int travelPerDay;       //交通每日补贴
    int travelSub;          //交通补贴，由天数x每日补贴自动获得

    QButtonGroup *groupBtn1;
};

#endif // ADDPERSONNEL_H
