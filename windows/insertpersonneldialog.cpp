#include "insertpersonneldialog.h"
#include "ui_insertpersonneldialog.h"

#include <QDebug>

insertPersonnelDialog::insertPersonnelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::insertPersonnelDialog)
{
    perNum = 0;
    personType = 1;

    totalCount = 0;
    currentAddedCount = 0;

    ui->setupUi(this);
}

insertPersonnelDialog::~insertPersonnelDialog()
{
    delete ui;
}

void insertPersonnelDialog::setType(int type)
{
    personType = type;
}

void insertPersonnelDialog::on_addBtn_clicked()
{
    QListWidgetItem *item = new QListWidgetItem;
    if (personType == 2)        //出国报销人员添加
    {
        item->setSizeHint(QSize(1000, 660));
        ui->listWidget->addItem(item);

        perNum += 1;
        abroadPersonnel *busiItem = new abroadPersonnel();
        busiItem->setIndex(perNum);

        ui->listWidget->setItemWidget(item, busiItem);

        connect(this, &insertPersonnelDialog::addAllAbroadPerson, busiItem, &abroadPersonnel::saveItem);
        connect(busiItem, &abroadPersonnel::added, this, &insertPersonnelDialog::addPerCount);
    }
    else                        //差旅报销人员添加
    {
        item->setSizeHint(QSize(1000, 560));
        ui->listWidget->addItem(item);

        perNum += 1;
        addPersonnel *busiItem = new addPersonnel();
        busiItem->setIndex(perNum);

        ui->listWidget->setItemWidget(item, busiItem);

        connect(this, &insertPersonnelDialog::addAllPerson, busiItem, &addPersonnel::saveItem);
        connect(busiItem, &addPersonnel::added, this, &insertPersonnelDialog::addPerCount);
    }
}

void insertPersonnelDialog::on_delBtn_clicked()
{
//    if (perNum == 0)
//        return;
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    ui->listWidget->removeItemWidget(currentItem);
    delete currentItem;

    perNum -= 1;
    if (perNum != ui->listWidget->count())
        qDebug() << "number error";

    //删除item的时候重新调整人员编号，有bug，后续进行修改
//    addPersonnel *item;
//    for (int i=1; i<=perNum; i++)
//    {
//        QListWidgetItem *listItem = ui->listWidget->item(i);
//        item =dynamic_cast<addPersonnel*>(ui->listWidget->itemWidget(listItem));
//        item->setIndex(i);
//    }
}

void insertPersonnelDialog::on_saveBtn_clicked()
{
    totalCount = ui->listWidget->count();

    if (personType == 2)        //出国报销人员添加
    {
        emit addAllAbroadPerson();
    }
    else                        //差旅报销人员添加
    {
        emit addAllPerson();
    }
}

void insertPersonnelDialog::on_exitBtn_clicked()
{
    close();
}

//复制人员信息，一栏信息里只有一次行程
void insertPersonnelDialog::on_copyBtn_clicked()
{

}

//添加一条人员信息的时候就将count加1,当达到人员的数量的时候，就可以认为是已经完成了，此时关闭这个窗口
void insertPersonnelDialog::addPerCount()
{
    if (totalCount = 0) {
        currentAddedCount = 0;
        close();
    }

    currentAddedCount ++;

    if(currentAddedCount >= totalCount) {
        totalCount = 0;
        currentAddedCount = 0;
        close();
    }
}
