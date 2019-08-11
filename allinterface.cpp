#include "allinterface.h"
#include <QDebug>
#include <QByteArray>
#include <QUrlQuery>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
allInterface *allInterface::instance =NULL;
allInterface::allInterface(QWidget *parent) : QWidget(parent)
{
  ipAddress = "http://211.157.179.73:9540";
  connect (this,SIGNAL(readUserDone()),this,SLOT(getuserdatalist()));
  managerJar = new QNetworkCookieJar(this);
}
allInterface:: ~allInterface()
{
    delete instance;
    instance = NULL;
}

allInterface *allInterface::getinstance ()
{
    if (NULL == instance) {
        instance = new allInterface();
    }
    return instance;
}


//发送用户信息get
void allInterface::userInterface ()
{
    qDebug()<<"调用用户接口";
    qDebug()<<"username:"<<info.getusername ();
    qDebug()<<"password:"<<info.getpassword ();


    QNetworkAccessManager *usermanager = new QNetworkAccessManager(this);
    QObject::connect (usermanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(readUserInterfaceinfo(QNetworkReply*)));
    QUrlQuery params;
    params.addQueryItem("username",info.getusername ());
    params.addQueryItem("password",info.getpassword ());
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/api/robot/login?").append (data.toUtf8()));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    QNetworkReply *reply = usermanager->get (request);
    usermanager->setCookieJar (managerJar);

}
//解析登录返回的信息
void allInterface::readUserInterfaceinfo (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray all = reply->readAll();
        QJsonParseError jsonError;
        QJsonObject dataObject;
        QString name;
        QString msg;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
//        qDebug()<< QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","")<<endl;
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
                                }
                                    emit readUserDone ();
                            }
                        }

                    }
                }
           }
        }
    }

}
//获取用户列表中的数据
void allInterface::getuserdatalist ()
{
    QNetworkAccessManager *userdatamanager = new QNetworkAccessManager(this);
    QObject::connect (userdatamanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setuserdatalist(QNetworkReply*)));
    QUrlQuery params;
    params.addQueryItem("name",info.getname ());
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/index/sys/user/dataList?").append (data.toUtf8()));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    userdatamanager->setCookieJar (managerJar);
    QNetworkReply *reply = userdatamanager->get (request);
}
//解析用户列表数据
void allInterface::setuserdatalist (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"getuserlistStatus"<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
        QByteArray all = reply->readAll();
//        qDebug()<<"printuserdatalist"<<all;
        QJsonParseError jsonError;
        QString officeName;
        QString userProvince;
        QString userCity;
        QString userBankName;
        QString userBankAccount;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
//        qDebug()<< QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","")<<endl;
        if (!doucment.isEmpty ()&&jsonError.error== QJsonParseError::NoError)
        {
            if(doucment.isObject ())
            {
                QJsonObject object = doucment.object ();
                if(object.contains ("data"))
                {
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
                            QJsonValue officeVal = dataObject.value ("office");
                            QJsonObject officeObject = officeVal.toObject ();
                            if(officeObject.contains ("name"))
                            {
                                officeName = officeObject.take ("name").toString ();
                                info.setofficeName (officeName);
                            }

                        }
                        if(dataObject.contains ("rank"))
                        {
                            QJsonValue rankVal = dataObject.value ("rank");
                            QJsonObject rankObject = rankVal.toObject ();
                            if(rankObject.contains ("id"))
                            {
                                QString id = rankObject.take ("id").toString ();
                                info.setRandId (id);
                            }
                            if(rankObject.contains ("rankName"))
                            {
                                QString rankName = rankObject.take ("rankName").toString ();
                                info.setrankName (rankName);
                            }
                        }
                    }


                }
            }
        }
     }
    emit setUserDataListDone ();
}
//获取院外人员信息
void allInterface::getDataPeopleOut ()
{
        QNetworkAccessManager *peopleOutmanager = new QNetworkAccessManager(this);
        QObject::connect (peopleOutmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setDatapeopleOut(QNetworkReply*)));
        QUrlQuery params;
        params.addQueryItem("name",info.getpeopleOutName ());
        QString  data = params.toString();
        QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/index/basedata/dataPeopleOut/dataList?").append (data.toUtf8()));
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
        peopleOutmanager->setCookieJar (managerJar);
        QNetworkReply *reply = peopleOutmanager->get (request);

}
//保存院外人员信息
void allInterface::setDatapeopleOut (QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QByteArray all;
    QUrl url;
    switch (statusCode) {
    case 200:
         all= reply->readAll();
         qDebug()<<"院外人员信息"<<all;
        break;
    case 302:
        url = reply->attribute (QNetworkRequest::RedirectionTargetAttribute).toUrl ();
        qDebug()<<"重定向url:"<<url;
        break;
    default:
        break;
    }
}
//获取项目信息.
void allInterface::getDataProject ()
{
        QNetworkAccessManager *DataProjectmanager = new QNetworkAccessManager(this);
        QObject::connect (DataProjectmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setDataProject(QNetworkReply*)));
        QUrlQuery params;
        params.addQueryItem("fundVersion",info.getfundVersion ());
        QString  data = params.toString();
        QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/index/basedata/dataProject/dataList?").append (data.toUtf8()));
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
        DataProjectmanager->setCookieJar (managerJar);
        QNetworkReply *reply = DataProjectmanager->get (request);

}
void allInterface::setDataProject (QNetworkReply *reply){
        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray all = reply->readAll();
            QString userprojectName;
            QString usermoneyOriginName;
            QJsonParseError jsonError;
            QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);

             qDebug()<<"printDataProject"<<doucment;
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

}
//国内差旅标准
void allInterface::getdataTravel ()
{
    QNetworkAccessManager *DataProjectmanager = new QNetworkAccessManager(this);
    QObject::connect (DataProjectmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setdataTravel(QNetworkReply*)));
    QUrlQuery params;
    params.addQueryItem("province",info.getarriveCity ());
    params.addQueryItem("rankName",info.getrankName ());
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/index/basedata/dataTravel/dataListJson?").append (data.toUtf8()));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    DataProjectmanager->setCookieJar (managerJar);
    QNetworkReply *reply = DataProjectmanager->get (request);

}
void allInterface::setdataTravel (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray all = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
        qDebug()<< "dataTravel信息"<<all;
         if (!doucment.isEmpty ()&&jsonError.error== QJsonParseError::NoError)
         {
             QJsonObject object = doucment.object ();
             if(object.contains ("data"))
             {
                 qDebug()<<"包含data";
                 QJsonValue dataVal = object.value ("data");
                 QJsonArray dataArray = dataVal.toArray ();
                 QJsonObject dataObject = dataArray.at (0).toObject ();

                 //QString id = dataObject.value ("id").toString ();

                 qDebug()<< "dataObject信息:"<<dataObject;
                 if(dataObject.contains ("staySubsidy"))
                 {
                     int userStaysidy = dataObject.take ("staySubsidy").toInt ();
                     info.setstaySubsidy (userStaysidy);
                     qDebug()<<"data包含staySubsidy"<<userStaysidy;
                 }
                 if(dataObject.contains ("trafficSubsidy"))
                 {
                     int userTrafficSubsidy = dataObject.take("trafficSubsidy").toInt ();
                     info.settrafficSubsidy (userTrafficSubsidy);
                     qDebug()<<"data包含trafficSubsidy:"<<userTrafficSubsidy;
                 }
                 if(dataObject.contains ("mealSubsidy"))
                 {
                     int userMealSubsidy = dataObject.take ("mealSubsidy").toInt ();
                     info.setmealSubsidy (userMealSubsidy);
                     qDebug()<<"data包含mealSubsidy:"<<userMealSubsidy;
                 }

                 emit setdataTravelDone ();

              }

         }
    }


}
void allInterface::getdataCountriesExpense ()
{
    QNetworkAccessManager *DataProjectmanager = new QNetworkAccessManager(this);
    QObject::connect (DataProjectmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setdataCountriesExpense(QNetworkReply*)));
    QUrlQuery params;
    params.addQueryItem("province",info.getarriveCity ());
    params.addQueryItem("rankId",info.getRandId ());
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequest(ipAddress.left(26).append("/erpbeidakouqiang/index/basedata/dataCountrisExpense/dataListJson?").append (data.toUtf8()));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    DataProjectmanager->setCookieJar (managerJar);
    QNetworkReply *reply = DataProjectmanager->get (request);
}
void allInterface::setdataCountriesExpense (QNetworkReply *reply)
{

    QByteArray all = reply->readAll();
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
    qDebug()<< "dataTravel信息"<<all;
     if (!doucment.isEmpty ()&&jsonError.error== QJsonParseError::NoError)
     {
         QJsonObject object = doucment.object ();
         if(object.contains ("data"))
         {
             qDebug()<<"包含data";
             QJsonValue dataVal = object.value ("data");
             QJsonArray dataArray = dataVal.toArray ();
             QJsonObject dataObject = dataArray.at (0).toObject ();
             QJsonValue  currencyVal = dataObject.value ("currency");
             QJsonObject currencyObject = currencyVal.toObject ();
             if(dataObject.contains ("staySubsidy"))
             {
                 int userStaysidy = dataObject.take ("staySubsidy").toInt ();
                 info.setaboradStaySubsidy (userStaysidy);
                 qDebug()<<"data包含staySubsidy"<<userStaysidy;
             }
             if(dataObject.contains ("commonMiscellaneous"))
             {
                 int userTrafficSubsidy = dataObject.take("commonMiscellaneous").toInt ();
                 info.setcommonMiscellaneous (userTrafficSubsidy);
                 qDebug()<<"data包含trafficSubsidy:"<<userTrafficSubsidy;
             }
             if(dataObject.contains ("mealSubsidy"))
             {
                 int userMealSubsidy = dataObject.take ("mealSubsidy").toInt ();
                 info.setaboradMealSubsidy (userMealSubsidy);
                 qDebug()<<"data包含mealSubsidy:"<<userMealSubsidy;
             }
             if(currencyObject.contains ("currencyName"))

             {
                 QString userCurrencyName = currencyObject.value ("currencyName").toString ();
                 info.setcurrencyName (userCurrencyName);

             }

             emit setdataCountriesExpenseDone ();

          }

     }

}
