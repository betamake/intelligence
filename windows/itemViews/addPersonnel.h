#ifndef ADDPERSONNEL_H
#define ADDPERSONNEL_H

#include <QWidget>
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
    void setNemberOnly();   //设置输入框只能输入数字

signals:
    void added();

private slots:
    void on_cleraBtn_clicked();

public slots:
    void saveItem();

private:
    Ui::addPersonnel *ui;

    traBusPersonInfo *traPerInfo;
};

#endif // ADDPERSONNEL_H
