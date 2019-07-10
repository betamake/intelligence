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
    void modifyPayItem(payItemInfo *info);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_saveEditBtn_clicked();

private:
    //规定金额处只能输入int类型
    void initInput();

private:
    Ui::addPayDialog *ui;

    int currentType;
    payItemInfo *curItem;

    bool beginModify;       //修改的标志
};

#endif // ADDPAYDIALOG_H
