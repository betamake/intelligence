#ifndef BILLITEM_H
#define BILLITEM_H

#include <QWidget>

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

private:
    Ui::billItem *ui;
};

#endif // BILLITEM_H
