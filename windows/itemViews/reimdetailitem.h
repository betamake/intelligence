#ifndef REIMDETAILITEM_H
#define REIMDETAILITEM_H

#include <QWidget>
#include "managers/reimdetailmanager.h"
#include "allinterface.h"
namespace Ui {
class reimDetailItem;
}

class reimDetailItem : public QWidget
{
    Q_OBJECT

public:
    explicit reimDetailItem(QWidget *parent = nullptr);
    ~reimDetailItem();

    void setReimDetail(reimDetail *info);
//    reimDetail *getReimDetail() { return detail;}

//    void setIndex(int index) { this->mIndex = index;}
//    int getIndex() { return this->mIndex;}

signals:
    void addReimItem(int index, reimDetail *info);

    void changeAccount();

//    void hadSaved(reimDetail *detail);

public slots:
    void saveDetail();

private slots:
    void on_searchBudgetBtn_clicked();

    void on_searchItemBtn_clicked();

    void on_searchDepartmentBtn_clicked();

    void setDataProjectInfo();

    void on_reimAccount_editingFinished();

private:
    Ui::reimDetailItem *ui;

    reimDetail *detail;

//    int mIndex;
};

#endif // REIMDETAILITEM_H
