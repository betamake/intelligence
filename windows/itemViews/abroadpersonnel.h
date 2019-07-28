#ifndef ABROADPERSONNEL_H
#define ABROADPERSONNEL_H

#include <QWidget>

//#include "windows/itemViews/abroadfeeitem.h"
#include "managers/personnelmanager.h"

namespace Ui {
class abroadPersonnel;
}


class abroadPersonnel : public QWidget
{
    Q_OBJECT

public:
    explicit abroadPersonnel(QWidget *parent = nullptr);
    ~abroadPersonnel();

    void setIndex(int index);

    void setPerId(int idIndex);

    void setPerson(abroadPersonInfo *info);

    abroadPersonInfo *getPerson() const { return abroadPerInfo;}


signals:
    void added();
    void addToMainWindow(int index, abroadPersonInfo *info);

public slots:
    void saveItem();

private slots:
    void getCoinType();
    void setValue();

    void getDays();

private:
    void initInput();
//    void addListItems();
    double chooseCoinType(QString &str);

private:
    Ui::abroadPersonnel *ui;

    abroadPersonInfo    *abroadPerInfo;

    int mIndex;

};

#endif // ABROADPERSONNEL_H
