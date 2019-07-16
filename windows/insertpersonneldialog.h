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


signals:
    void addAllPerson();        //添加差旅人员的信号
    void addAllAbroadPerson();     //添加出国人员的信号

private slots:
    void on_addBtn_clicked();
    void on_delBtn_clicked();
    void on_saveBtn_clicked();
    void on_exitBtn_clicked();

    void on_copyBtn_clicked();

    void addPerCount();         //添加一条人员信息的时候就将count加1,当达到人员的数量的时候，就可以认为是已经完成了，此时关闭这个窗口

private:
    Ui::insertPersonnelDialog *ui;

    int personType;         //人员类型，1为出差，2为出国

    int perNum;             //人员数
    int currentIndex;       //当前被选中的人员

    QList<addPersonnel*> traPerList;
    QList<abroadPersonnel*> abroadPerList;

    int totalCount;         //人员的总数
    int currentAddedCount;  //现在已经添加的人员的数量
};

#endif // INSERTPERSONNELDIALOG_H
