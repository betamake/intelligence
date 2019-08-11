#ifndef ABROADPERSONNEL_H
#define ABROADPERSONNEL_H

#include <QWidget>

//#include "windows/itemViews/abroadfeeitem.h"
#include "managers/personnelmanager.h"
#include "allinterface.h"
namespace Ui {
class abroadPersonnel;
}


class abroadPersonnel : public QWidget
{
    Q_OBJECT

public:
    explicit abroadPersonnel(QWidget *parent = nullptr);
    ~abroadPersonnel();

    void setIndex(int index);


signals:
    void added();

public slots:
    void saveItem();

private slots:
    void getCoinType();
    void setValue();

    void getDays();

    void on_searchStaffBtn_clicked();

    void on_staffNumber_editingFinished();
    void dealUserData();

    void on_arriveCity_editingFinished();
    void dealdataCountriesExpense();

private:
    void initInput();
//    void addListItems();
    double chooseCoinType(QString &str);

private:
    Ui::abroadPersonnel *ui;

    abroadPersonInfo    *abroadPerInfo;

    int count;      //添加外币人民币的次数
};

#endif // ABROADPERSONNEL_H
