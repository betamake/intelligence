#ifndef PAYMETHODSITEM_H
#define PAYMETHODSITEM_H

#include <QWidget>
#include "payinfomanager.h"

namespace Ui {
class payMethodsItem;
}

class payMethodsItem : public QWidget
{
    Q_OBJECT

public:
    explicit payMethodsItem(QWidget *parent = nullptr);
    ~payMethodsItem();

    void setCurrentIndex(int index);

    void setInfoItem(payItemInfo *info);

    payItemInfo *getInfoItem();

private slots:

    void on_delBtn_clicked();

    void on_modifyBtn_clicked();

private:
    Ui::payMethodsItem *ui;

    payItemInfo *curItem;
    int currentIndex;
};

#endif // PAYMETHODSITEM_H