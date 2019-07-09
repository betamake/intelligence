#ifndef REIMDETAILITEM_H
#define REIMDETAILITEM_H

#include <QWidget>

namespace Ui {
class reimDetailItem;
}

class reimDetailItem : public QWidget
{
    Q_OBJECT

public:
    explicit reimDetailItem(QWidget *parent = nullptr);
    ~reimDetailItem();

private:
    Ui::reimDetailItem *ui;
};

#endif // REIMDETAILITEM_H
