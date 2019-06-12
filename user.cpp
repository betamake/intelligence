#include "user.h"
#include<QDebug>
User::User(){

}
User::User( const QString uid,
            const QVariant avater,
            const QString authoId,
            const QString username,
            const QString password,
            const QString idFace,
            const QString idCard,
            const QString phoneNumber,
            const QString createTime,
            const QString updateTime)
{
    setUid(uid);
    setAvater(avater);
    setAuthoId(authoId);
    setUsername(username);
    setPassword(password);
    setIdFace(idFace);
    setIdCard(idCard);
    setPhoneNumber(phoneNumber);
    setCreateTime(createTime);
    setUpdateTime(updateTime);
}
User::~User()
{
}
QString User::getUid()
{
    return uid;
}
void User::setUid(const QString uid)
{
    this->uid = uid;
}
QVariant User::getAvater()
{
    return avater;
}
void User::setAvater(const QVariant avater)
{
    this->avater = avater;
}
QString User::getAuthoId()
{
    return authoId;
}
void User::setAuthoId(const QString authoId)
{
    this->authoId = authoId;
}

QString User::getUsername()
{
    return username;
}
void User::setUsername(const QString username)
{
    this->username = username;
}

QString User::getPassword()
{
    return password;
}
void User::setPassword(const QString password)
{
    this->password = password;
}


QString User::getIdFace()
{
    return idFace;
}
void User::setIdFace(const QString idFace)
{
    this->idFace = idFace;
}

QString User::getIdCard()
{
    return idCard;
}
void User::setIdCard(const QString idCard)
{
    this->idCard = idCard;
}

void User::setPhoneNumber(const QString phoneNumber)
{
    this->phoneNumber = phoneNumber;
}
QString User::getPhoneNumber()
{
    return phoneNumber;
}

void User::setCreateTime(const QString createTime)
{
    this->createTime = createTime;
}
QString User::getCreateTime()
{
    return createTime;
}
void User::setUpdateTime(const QString updateTime)
{
    this->updateTime = updateTime;
}
QString User::getUpdateTime()
{
    return updateTime;
}
QString User::toString()
{
    //    QString id;//支付方式 待定
    //    QString username;//收款人
    //    QString password;//所属银行
    //    QString idFace;//开户行名称
    //    QString idCard;//银行卡号
    //    QString phoneNumber;//金额
    //    QString createTime;//备注
    return "[username:"+username+
            ",uid:"+uid+
            //            ",avater:"+avater+
            ", authoId:"+authoId+
            ",password:"+password+
            ", idface:"+idFace+
            ",idCard:"+idCard+
            ", phoneNumber:"+phoneNumber+
            ",updateTime:"+updateTime+
            ",createTime:"+createTime+
            "]";
}
