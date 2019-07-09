#ifndef REIMDETAILITEM_H
#define REIMDETAILITEM_H

#include <QWidget>
#include "reimdetailmanager.h"

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

private:
    Ui::reimDetailItem *ui;

    reimDetail *detail;
};

#endif // REIMDETAILITEM_H
