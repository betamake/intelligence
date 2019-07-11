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

    int iRow;
};

#endif // BILLITEM_H
