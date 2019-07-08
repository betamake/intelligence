#ifndef INSERTPERSONNELDIALOG_H
#define INSERTPERSONNELDIALOG_H

#include <QDialog>
#include "windows/itemViews/addPersonnel.h"
#include "windows/itemViews/abroadpersonnel.h"

namespace Ui {
class insertPersonnelDialog;
}

class insertPersonnelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit insertPersonnelDialog(QWidget *parent = nullptr);
    insertPersonnelDialog(int type, QWidget *parent = nullptr);
    ~insertPersonnelDialog();

    void setType(int type);

private:


private slots:
    void on_addBtn_clicked();
    void on_delBtn_clicked();
    void on_saveBtn_clicked();
    void on_exitBtn_clicked();



    void on_copyBtn_clicked();

private:
    Ui::insertPersonnelDialog *ui;

    int personType;         //人员类型，1为出差，2为出国

    int perNum;             //人员数
    int currentIndex;       //当前被选中的人员

    QList<addPersonnel*> traPerList;
    QList<abroadPersonnel*> abroadPerList;
};

#endif // INSERTPERSONNELDIALOG_H
