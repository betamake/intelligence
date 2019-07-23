#include "allinterface.h"
#include <QDebug>
#include <QByteArray>
#include <QUrlQuery>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
allInterface::allInterface(QWidget *parent) : QWidget(parent)
{
//    info = new InterfaceInfo();
  ipAddress = "http://211.157.179.73:9540";
  connect (this,SIGNAL(readUserDone()),this,SLOT(getuserdatalist()));
  managerJar = new QNetworkCookieJar;


}
//发送用户信息get
void allInterface::userInterface ()
{
    qDebug()<<"调用用户接口";
    qDebug()<<"username:"<<info.getusername ();
    qDebug()<<"password:"<<info.getpassword ();


    manager = new QNetworkAccessManager(this);
    QObject::connect (manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(readUserInterfaceinfo(QNetworkReply*)));
    QUrlQuery params;
    params.addQueryItem("username",info.getusername ());
    params.addQueryItem("password",info.getpassword ());
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/api/robot/login?").append (data.toUtf8()));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    QString url =ipAddress.left(26).append("/erpbeidakouqiang/api/robot/login?").append (data.toUtf8());
    manager->setCookieJar (managerJar);
    QNetworkReply *reply = manager->get (request);
}
//解析登录返回的信息
void allInterface::readUserInterfaceinfo (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray all = reply->readAll();
        qDebug()<<"111111111111"<<all;
        QJsonParseError jsonError;
        QJsonObject dataObject;
        QString name;
        QString msg;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
        qDebug()<< QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","")<<endl;
        if((!doucment.isEmpty()) && jsonError.error == QJsonParseError::NoError)
        {
            if(doucment.isObject())
            {
                QJsonObject object = doucment.object();
                if(object.contains ("msg"))
                {
                    QJsonValue dataVal = object.take("msg");
                    msg = dataVal.toString ();
                    info.setmsg (msg);
                    qDebug()<<"msg"<<info.getmsg ();
                    if (msg =="success")
                    {
                        if(object.contains("data"))
                        {
                            QJsonValue dataVal = object.value ("data");
                            if(dataVal.isObject ())
                            {
                                dataObject = dataVal.toObject ();
                                if(dataObject.contains ("name")){
                                    QJsonValue nameVal = dataObject.take ("name");
                                    name = nameVal.toString ();
                                    info.setname (name);
                                    qDebug()<<"name:"<<info.getname ();
                                }
                            }
                        }

                    }
                }
           }
        }
    }
    emit readUserDone ();

}
//获取用户列表中的数据
void allInterface::getuserdatalist ()
{
    manager = new QNetworkAccessManager(this);
    QObject::connect (manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setuserdatalist(QNetworkReply*)));
    QUrlQuery params;
    params.addQueryItem("name",info.getname ());
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/index/sys/user/dataList?").append (data.toUtf8()));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->setCookieJar (managerJar);
    QNetworkReply *reply = manager->get (request);
}
//解析用户列表数据
void allInterface::setuserdatalist (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray all = reply->readAll();
        qDebug()<<"111111111111"<<all;
        QJsonParseError jsonError;
        QString officeName;
        QString userProvince;
        QString userCity;
        QString userBankName;
        QString userBankAccount;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
        qDebug()<< QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","")<<endl;
        if (!doucment.isEmpty ()&&jsonError.error== QJsonParseError::NoError)
        {
            qDebug()<<"document不为空";
            if(doucment.isObject ())
            {
                qDebug()<<"doucment是Obejct";
                QJsonObject object = doucment.object ();
                if(object.contains ("data"))
                {
                    qDebug()<<"Obejct包含data";
                    QJsonValue dataVal  = object.value ("data");
                    QJsonArray dataArray = dataVal.toArray ();
                    for (int i = 0;i<dataArray.size ();i++)
                    {
                        QJsonValue dataVal=dataArray.at (i);
                        QJsonObject dataObject = dataVal.toObject ();
                        QJsonValue userAcountListVal = dataObject.value ("userAccountList");
                        QJsonArray userAcountListArray = userAcountListVal.toArray ();
                        for (int i =0;i<userAcountListArray.size ();i++)
                        {
                            QJsonValue userAcountListVal = userAcountListArray.at (i);
                            QJsonObject userAcountListObject = userAcountListVal.toObject ();
                            if(userAcountListObject.contains ("province"))
                            {
                                userProvince = userAcountListObject.take ("province").toString ();
                                info.setprovince (userProvince);
                                qDebug()<<"userprovince:"<<info.getprovice ();

                            }
                            if(userAcountListObject.contains ("city"))
                            {
                                userCity = userAcountListObject.take("city").toString ();
                                info.setcity (userCity);
                            }
                            if(userAcountListObject.contains ("bankName"))
                            {
                                userBankName = userAcountListObject.take ("bankName").toString ();
                                info.setbankName (userBankName);
                            }
                            if(userAcountListObject.contains ("bankAccount"))

                            {
                                userBankAccount = userAcountListObject.take ("bankAccount").toString ();
                                info.setbankAccount (userBankAccount);
                            }
                        }
                        if(dataObject.contains ("office"))
                        {
                            qDebug()<<"dataobject包含office";
                            QJsonValue officeVal = dataObject.value ("office");
                            QJsonObject officeObject = officeVal.toObject ();
                            if(officeObject.contains ("name"))
                            {
                                officeName = officeObject.take ("name").toString ();
                                info.setofficeName (officeName);
                                qDebug()<<"部门名称:"<<info.getofficeName ();
                            }

                        }
                    }


                }
            }
        }
     }
}
////获取院外人员信息
//void allInterface::getDataPeopleOut ()
//{
//    manager = new QNetworkAccessManager(this);
//    QObject::connect (manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setDatapeopleOut(QNetworkReply*)));
//    QUrlQuery params;
//    params.addQueryItem("name",info.getname ());
//    QString  data = params.toString();
//    QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/index/basedata/dataPeopleOut/dataList?").append (data.toUtf8()));
//    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
//    manager->setCookieJar (managerJar);
//    QNetworkReply *reply = manager->get (request);
//}
////保存院外人员信息
//void allInterface::setDatapeopleOut (QNetworkReply *reply)
//{
//    if(reply->error() == QNetworkReply::NoError)
//    {
//        QByteArray all = reply->readAll();
//        qDebug()<<"111111111111"<<all;
//}
void allInterface::getDataProject ()
{
    manager = new QNetworkAccessManager(this);
    QObject::connect (manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setDataProject(QNetworkReply*)));
    QUrlQuery params;
    params.addQueryItem("fundVersion",info.getfundVersion ());
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/index/basedata/dataProject/dataList?").append (data.toUtf8()));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->setCookieJar (managerJar);
    QNetworkReply *reply = manager->get (request);
}
void allInterface::setDataProject (QNetworkReply *reply){
        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray all = reply->readAll();
            qDebug()<<"111111111111"<<all;
            QString userprojectName;
            QString usermoneyOriginName;
            QJsonParseError jsonError;
            QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
            if (!doucment.isEmpty ()&&jsonError.error== QJsonParseError::NoError)
            {
                QJsonObject object = doucment.object ();
                if(object.contains ("data"))
                {
                    QJsonValue dataVal = object.value ("data");
                    QJsonArray dataArray = dataVal.toArray ();
                    QJsonObject dataObject = dataArray.at (0).toObject ();
                    if(dataObject.contains ("projectName"))
                    {
                        userprojectName = dataObject.take ("projectName").toString ();
                        info.setprojectName (userprojectName);
                    }
                    if(dataObject.contains ("moneyOriginName"))
                    {
                        usermoneyOriginName = dataObject.take ("moneyOriginName").toString ();
                        info.setmoneyOriginName (usermoneyOriginName);
                    }
                }

            }

        }
        emit setDataProjectDone ();

}
void allInterface::getDataReimberse ()
{
    manager = new QNetworkAccessManager(this);
    QObject::connect (manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setDataReimberse(QNetworkReply*)));
    QUrlQuery params;
    params.addQueryItem("reimburseName",info.getfundVersion ());
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/index/basedata/dataReimburse/dataList?").append (data.toUtf8()));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->setCookieJar (managerJar);
    QNetworkReply *reply = manager->get (request);
}
void allInterface::setDataReimberse (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray all = reply->readAll();
        qDebug()<<"111111111111"<<all;
        QString userReimburseName;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
        if(!doucment.isEmpty ()&&jsonError.error==QJsonParseError::NoError )
        {
            QJsonObject object = doucment.object ();
            if(object.contains ("data"))
            {
                QJsonValue dataVal = object.value ("data");
                QJsonArray dataArray = dataVal.toArray ();
                QJsonObject dataObject = dataArray.at (0).toObject ();
            }
        }
    }

}
