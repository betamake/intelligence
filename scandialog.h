#ifndef SCANDIALOG_H
#define SCANDIALOG_H

#include <QDialog>
#include <QThread>
#include "scanthread.h"
#include "billcheck.h"
namespace Ui {
class scanDialog;
}

class scanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit scanDialog(QWidget *parent = 0);
    ~scanDialog();

    void setBillCheck(BillCheck info);

signals:
    void scanDone();


private slots:
    void on_buttonScan_clicked();


    void on_buttonClose_clicked();
    void dealDone();
    void dealThread();

private:
    Ui::scanDialog *ui;
    ScanThread *thread;

    BillCheck bill;
};


#endif // SCANDIALOG_H
