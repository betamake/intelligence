#ifndef ADDPAYDIALOG_H
#define ADDPAYDIALOG_H

#include <QDialog>
#include "payinfomanager.h"

namespace Ui {
class addPayDialog;
}

class addPayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addPayDialog(QWidget *parent = nullptr);
    ~addPayDialog();

    payItemInfo *getPayItem() { return curItem;}

    void setItem(payItemInfo *info);

signals:
    void addPayItem(payItemInfo *info);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_saveEditBtn_clicked();

private:


private:
    Ui::addPayDialog *ui;

    int currentType;
    payItemInfo *curItem;
};

#endif // ADDPAYDIALOG_H
