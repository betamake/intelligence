#ifndef BILLITEM_H
#define BILLITEM_H

#include <QWidget>

#include "billcheck.h"

namespace Ui {
class billItem;
}

class billItem : public QWidget
{
    Q_OBJECT

public:
    explicit billItem(QWidget *parent = nullptr);
    ~billItem();

    void setBillType(const int type);
    void setBillAccount(const QString &money);
    void setBillPixmap(QPixmap image);
    void setBillInfo(BillCheck bill) { mBill = bill;}
    void setDeparture(const QString &departure);
    void setDestination(const QString &destination);

    void setIndex(int row);         //该item的行号

    BillCheck getBillInfo() { return mBill;}

signals:
    void openBillItem(BillCheck bill);
    void deleteBillItem(int row);

private slots:
    void on_modifyBtn_clicked();
    void on_deleteBtn_clicked();

private:
    Ui::billItem *ui;

    BillCheck mBill;
    int billType;           //票据的类型，当为火车票和飞机票的时候，要显示始发站和到达站; 1为发票，2为火车票，3为机票，4为出租车票

    int iRow;
};

#endif // BILLITEM_H
