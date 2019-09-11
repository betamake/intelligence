#ifndef PERSONITEM_H
#define PERSONITEM_H

#include <QWidget>

namespace Ui {
class personItem;
}

class personItem : public QWidget
{
    Q_OBJECT

public:
    explicit personItem(QWidget *parent = nullptr);
    ~personItem();

    void setPersonName(QString &strName);
    void setAccount(int account);
    void setDeparture(QString &departure);
    void setDestination(QString &destination);

private:
    Ui::personItem *ui;
};

#endif // PERSONITEM_H
