#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QString>
#include "user.h"
//#include "payinfo.h"
#include "billcheck.h"
#include "Information.h"
#include <string>
#include <QMessageBox>
#include <QDebug>
#include <costbaseinfo.h>
#include "connectionpool.h"
#include "scandialog.h"
#include <QObject>

class databaseUtils{
public:
     void createConnectionByName(const QString &connectionName);//初始化数据库
     QSqlDatabase getConnectionByName(const QString &connectionName);
     bool verificationLogin(QString username, QString password);
     void outputUserInfo();
     QString md5Encode(QString string);
     bool insertUser(User user);
     bool insertCostBaseInfo(costBase costBinfo);//插入费用报销信息
     /* 无用信息
 //    bool insertBusiexp(busiExp busiexp);//插入出差报销单信息
 //    bool insertAbdexp(abroadExp abdexp);//插入出国报销单信息
 //    bool insertCostexp(costExp costexp);//插入费用报销单信息
 //    bool insertExpensehead(ExpenseHead exphead);//插入表头信息
 //    bool insertPayinfo(PayInfo payinfo);//插入支付信息
 */
     bool insertBillcheck(BillCheck billinfo);

     bool checkUsername(QString username);//检查用户名是否重复
 //    bool checkUserByIdCard(QString idCard);//修改用户信息
     bool CheckUserByIdCard(QString idCard);//修改用户信息
     bool CheckUserByUid(QString uid);//检查用户名是否重复
     bool deleteUser(QString username);//删除用户信息
     bool updateUser(User user);//修改用户信息
     User SearchUserByUsername(QString username);//获取用户名
//     User SearchUidByUsername(QString username);//获取Uid
//     User SearchIdcardByUsername(QString username);//获取idcard

     User SearchUserByIdCard(QString idNumber);//修改用户信息
     User SearchUserByUid(QString uid);//修改用户信息
     User SearchUserByIdNumber(QString idCard);//修改用户信息

     //idcard表查询
     bool insertIdCard(Information information);//插入身份证信息
     bool checkIdCargByIdNumber(QString idNumber);//修改用户信息
     bool deleteIdCard(QString idNumber);//删除用户信息
     bool updateIdCard(Information information);//修改用户信息
     Information searchIdCardByIdNumber(QString idNumber);//修改用户信息

};

class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = 0);

signals:

public slots:
    bool insertBillInfo(billInfo billinfo);
};

#endif // DATABASE_H
