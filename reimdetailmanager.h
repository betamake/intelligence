#ifndef REIMDETAILMANAGER_H
#define REIMDETAILMANAGER_H

#include <QObject>

/**
* @brief       用来管理报销明细的
* @author      黄梦君
* @date        2019-07-09
*/

class reimDetail {

public:
    void setBudgetNumber(const QString &number) { this->budgetNumber = number;}
    void setBudgetName(const QString &name) { this->budgetName = name;}
    void setItem(const QString &item) { this->item = item;}
    void setDetails(const QString &details) { this->details = details;}
    void setDepartment(const QString &department) { this->department = department;}
    void setFeeType(const QString &type) { this->feeType = type;}
    void setAccount(int account) { this->account = account;}

    QString getBudgetNumber() const { return this->budgetNumber;}
    QString getBudgetName() const { return this->budgetName;}
    QString getItem() const { return this->item;}
    QString getDetails() const { return this->details;}
    QString getDepartment() const { return this->department;}
    QString getFeeType() const { return this->feeType;}
    int getAccount() {return this->account;}

private:
    QString budgetNumber;       //预算项目号/经费号
    QString budgetName;         //预算项目名称
    QString item;               //业务事项
    QString details;            //项目明细
    QString department;         //使用科室
    QString feeType;            //经费类型
    int account;                //报销金额
};

class reimDetailManager : public QObject
{
    Q_OBJECT
public:
    explicit reimDetailManager(QObject *parent = nullptr);

    void addReimDetail(reimDetail *info);       //添加一条报销明细
    void delReimDetail(reimDetail *info);       //删除一条报销明细

    void clearList();                           //清空报销明细

    QList<reimDetail*> getReimDetialList() { return detailList;}        //获得所有保存下来的报销明细

signals:

public slots:


private:
    QList <reimDetail*> detailList;         //报销明细列表
};

#endif // REIMDETAILMANAGER_H
