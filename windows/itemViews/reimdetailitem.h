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

    void setReimDetail(reimDetail *info) { detail = info;}
    reimDetail *getReimDetail() { return detail;}

public slots:
    void saveDetail();

private slots:
    void on_searchBudgetBtn_clicked();

    void on_searchItemBtn_clicked();

    void on_searchDepartmentBtn_clicked();
    void setDataProjectInfo();

private:
    Ui::reimDetailItem *ui;

    reimDetail *detail;
    allInterface *interface;
};

#endif // REIMDETAILITEM_H
