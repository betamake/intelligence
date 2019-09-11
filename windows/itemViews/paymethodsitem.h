#ifndef PAYMETHODSITEM_H
#define PAYMETHODSITEM_H

#include <QWidget>
#include "managers/payinfomanager.h"

namespace Ui {
class payMethodsItem;
}

class payMethodsItem : public QWidget
{
    Q_OBJECT

public:
    explicit payMethodsItem(QWidget *parent = nullptr);
    ~payMethodsItem();

    void setInfoItem(payItemInfo *info);

    payItemInfo *getInfoItem() { return this->curItem;}

private:
    Ui::payMethodsItem *ui;

    payItemInfo *curItem;
};

#endif // PAYMETHODSITEM_H
