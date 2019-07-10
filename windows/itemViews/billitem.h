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

    void setBillType(const QString &str);

    void setBillAccount(const QString &money);

    void setBillPixmap(QPixmap image);

    void setBillInfo(BillCheck bill) { mBill = bill;}

    BillCheck getBillInfo() { return mBill;}

signals:
    void openBillItem(BillCheck bill);

private slots:
    void on_modifyBtn_clicked();

private:
    Ui::billItem *ui;

    BillCheck mBill;
};

#endif // BILLITEM_H
