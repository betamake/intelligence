#ifndef ABROADFEEITEM_H
#define ABROADFEEITEM_H

#include <QWidget>

namespace Ui {
class abroadFeeItem;
}

class abroadFeeItem : public QWidget
{
    Q_OBJECT

public:
    explicit abroadFeeItem(QWidget *parent = nullptr);
    ~abroadFeeItem();

    double countValue();
    //清空金额输入栏
    void clearInput();

private:
    //设置汇率值
    void getType();
    //初始化金额输入框，只允许输入数字，人民币金额栏不允许编辑
    void initInput();

private:
    Ui::abroadFeeItem *ui;
    double mHuilv;
};

#endif // ABROADFEEITEM_H
