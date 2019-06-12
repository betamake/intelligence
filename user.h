#ifndef USER_H
#define USER_H
#include <QString>
#include<iostream>
#include<QString>
#include <QVariant>
#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif
using namespace std;
//用户表
class User
{
public:
    User();
    ~User();
    User( const QString uid,
          const QVariant avater,
          const QString authoId,
          const QString username,
          const QString password,
          const QString idFace,
          const QString idCard,
          const QString phoneNumber,
          const QString createTime,
          const QString updateTime);
    QString getUid();
    void setUid(const QString uid);

    QString getAuthoId();
    void setAuthoId(const QString authoId);

    QVariant getAvater();
    void setAvater(const QVariant avater);

    QString getUsername();
    void setUsername(const QString username);

    QString getPassword();
    void setPassword(const QString password);

    QString getIdFace();
    void setIdFace(const QString idFace);

    QString getIdCard();
    void setIdCard(const QString idCard);

    QString getPhoneNumber();
    void setPhoneNumber(const QString phoneNumber);

    QString getCreateTime();
    void setCreateTime(const QString createTime);

    QString getUpdateTime();
    void setUpdateTime(const QString updateTime);

    QString toString();


private:
    QString uid;
    QVariant avater;
    QString authoId;
    QString username;
    QString password;
    QString idFace;
    QString idCard;
    QString phoneNumber;
    QString createTime;
    QString updateTime;

};

#endif // USER_H
