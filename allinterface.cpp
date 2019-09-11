#include "allinterface.h"
#include <QDebug>
#include <QByteArray>
#include <QUrlQuery>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include<QCoreApplication>
allInterface *allInterface::instance =NULL;
allInterface::allInterface(QWidget *parent) : QWidget(parent)
{
  ipAddress = "http://211.157.179.73:9580";
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

void allInterface::getPubKeys ()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QObject::connect (manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setPubkeys(QNetworkReply*)));
    QNetworkRequest request = commonutils.getHttpRequestRemote(ipAddress.left(26).append("keys"));
    QNetworkReply *reply =manager->get (request);
}
void allInterface::setPubkeys (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray all = reply->readAll();
        QJsonParseError jsonError;
        QJsonObject dataObject;
        QString curPubExep;
        QString curModulus;
        QString curPubKeys;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
        if((!doucment.isEmpty()) && jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject object = doucment.object ();
            QJsonValue dataVal = object.value ("data");
            dataObject = dataVal.toObject ();
            QJsonValue curPubExepVal = dataObject.take ("pubExep");
            curPubExep=curPubExepVal.toString ();
            info.setpubExep (curPubExep);
            QJsonValue curModulusVal = dataObject.take ("modulus");
            curModulus = curModulusVal.toString ();
            info.setmodulus (curModulus);
            QJsonValue curPubKeysVal = dataObject.take ("pubKeys");
            curPubKeys=curPubKeysVal.toString ();
            info.setpubKeys (curPubKeys);

        }

    }

}

//发送用户信息get
void allInterface::userInterface ()
{
    qDebug()<<"调用用户接口";
    qDebug()<<"username:"<<info.getusername ();
    qDebug()<<"password:"<<info.getpassword ();


    QNetworkAccessManager *usermanager = new QNetworkAccessManager(this);
    QObject::connect (usermanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(readUserInterfaceinfo(QNetworkReply*)));
    QString password = "altcZOb9rUgFxUYSKzbbf6RqQr2ux/bTfmFJcd4grRENkf3P57bYN2K9PCvtKyXk8YTDep7NzzqKDwLcEbdDQ2tqMN1vq2ugOt3s332JqcKRlzsl4FfnRMfVwSN7dkwJUsbmLsNNV7cTejHoDhege3yxmtlOjyQQKnoSIIZMlBQ=";
    QString pubKey = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCilhfz9OQGhVsVeRPpw0b5B13TDan2ppM6fBMEwD5GdGn8+sjegs9DhbDTogcki4jF7H3nESl3d3yckqHT3ykR/Bwd+GoZFPqqR76L5+d8UNRsOodXFG5EsP/l2/JpCSvPJAeCq+a0tQS4JACUWBkCe11KtsZ/HyS8l/NKsg3GywIDAQAB";
    QString username = "005";
    QJsonObject loginJsonObject;
    loginJsonObject.insert ("password",password);
    loginJsonObject.insert ("pubKey",pubKey);
    loginJsonObject.insert ("username",username);
    QJsonDocument document;
    document.setObject (loginJsonObject);
    QByteArray loginArray = document.toJson (QJsonDocument::Compact);
    QNetworkRequest request = commonutils.getHttpRequestRemote(ipAddress.left(26).append("/1ogin"));
    QNetworkReply *reply = usermanager->post (request,loginArray);
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
                    if (msg =="登陆成功")
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
    params.addQueryItem("type","1");
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequestRemote(ipAddress.left(26).append("/datas/staff/select?").append (data.toUtf8()));
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

                        QJsonValue userAcountListVal = dataObject.value ("bankList");
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
                            //此处不确定参数的含义
                            if(userAcountListObject.contains ("municipality"))
                            {
                                userCity = userAcountListObject.take("municipality").toString ();
                                info.setcity (userCity);
                            }
                            if(userAcountListObject.contains ("bankName"))
                            {
                                userBankName = userAcountListObject.take ("bankName").toString ();
                                info.setbankName (userBankName);
                            }
                            if(userAcountListObject.contains ("bankNum"))

                            {
                                userBankAccount = userAcountListObject.take ("bankNum").toString ();
                                info.setbankAccount (userBankAccount);
                            }
                        }
                        if(dataObject.contains ("departName"))
                        {
                            officeName = dataObject.take ("departName").toString ();
                            info.setofficeName (officeName);
                        }
                        if(dataObject.contains ("rankId"))
                        {
                            QString id = dataObject.take ("rankId").toString ();
                            info.setRandId (id);
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
        QNetworkRequest request = commonutils.getHttpRequestRemote(ipAddress.left(26).append("/erpbeidakouqiang/index/basedata/dataPeopleOut/dataList?").append (data.toUtf8()));
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
        params.addQueryItem("name",info.getprojectName ());
        QString  data = params.toString();
        QNetworkRequest request = commonutils.getHttpRequestRemote(ipAddress.left(26).append("/datas/project/getBySearch?").append (data.toUtf8()));
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
    params.addQueryItem("rankId",info.getRandId ());
    params.addQueryItem("dateBegin",info.getdateBegin ());
    params.addQueryItem("dateEnd",info.getdateEnd ());
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequestRemote(ipAddress.left(26).append("/datas/travel/getStandard?").append (data.toUtf8()));
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
                 QJsonObject dataObject = dataVal.toObject ();

                 //QString id = dataObject.value ("id").toString ();

                 qDebug()<< "dataObject信息:"<<dataObject;
                 if(dataObject.contains ("staysubsidy"))
                 {
                     int userStaysidy = dataObject.take ("staysubsidy").toInt ();
                     info.setstaySubsidy (userStaysidy);
                     qDebug()<<"data包含staySubsidy"<<userStaysidy;
                 }
                 if(dataObject.contains ("trafficsubsidy"))
                 {
                     int userTrafficSubsidy = dataObject.take("trafficsubsidy").toInt ();
                     info.settrafficSubsidy (userTrafficSubsidy);
                     qDebug()<<"data包含trafficSubsidy:"<<userTrafficSubsidy;
                 }
                 if(dataObject.contains ("mealsubsidy"))
                 {
                     int userMealSubsidy = dataObject.take ("mealsubsidy").toInt ();
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
    QNetworkRequest request = commonutils.getHttpRequestRemote(ipAddress.left(26).append("/erpbeidakouqiang/index/basedata/dataCountrisExpense/dataListJson?").append (data.toUtf8()));
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
void allInterface::getUploadFile ()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QObject::connect (manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setUploadFile(QNetworkReply*)));
    QUrlQuery params;
    QFile file(info.getuploadPath ());
    if (!file.open(QIODevice::ReadOnly)||file.size()==0)
    {
        file.close();
        return ;
    }
    QByteArray fdata = file.readAll();
    file.close();
    fdata = fdata.toBase64();
    fdata = fdata.replace("+","-");
    fdata = fdata.replace("/","_");
    params.addQueryItem("file",fdata);
    QString  data = params.toString();
    QNetworkRequest request = commonutils.getHttpRequestRemote(ipAddress.left(26).append("/admin/attachment/pro/upload?").append (data.toUtf8()));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->setCookieJar (managerJar);
    QNetworkReply *reply = manager->post(request,data.toUtf8 ());
}
void allInterface::setUploadFile (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray all = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
         if (!doucment.isEmpty ()&&jsonError.error== QJsonParseError::NoError)
         {
             QJsonObject object = doucment.object ();
             if(object.contains ("data"))
             {
                 qDebug()<<"包含data";
                 QJsonValue dataVal = object.value ("data");
                 QJsonObject dataObject = dataVal.toObject ();
                 qDebug()<< "dataObject信息:"<<dataObject;
                 if(dataObject.contains ("name"))
                 {
                     QString name = dataObject.take ("name").toString ();
                     info.setuploadName (name);
                     qDebug()<<"文件name"<<name;

                 }
                 if(dataObject.contains ("path"))
                 {
                     QString path = dataObject.take ("path").toString ();
                     info.setuploadPath (path);

                 }
             }
         }
    }
}
void allInterface::getIdCardThread ()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QObject::connect (manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(setIdCardThread(QNetworkReply*)));
    QJsonObject loginJsonObject;
    loginJsonObject.insert ("username",info.getidCardInformation ());
    QJsonDocument document;
    document.setObject (loginJsonObject);
    QByteArray loginArray = document.toJson (QJsonDocument::Compact);
    QNetworkRequest request = commonutils.getHttpRequestRemote(ipAddress.left(26).append("/robot/1ogin"));
    QNetworkReply *reply = manager->post (request,loginArray);
    manager->setCookieJar (managerJar);
}
void allInterface::setIdCardThread (QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray all = reply->readAll();
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
                }
                if(object.contains("data"))
                {
                    qDebug()<<"data"<<info.getmsg ();
                    QJsonValue dataVal = object.value ("data");
                    if(dataVal.isObject ())
                    {
                        dataObject = dataVal.toObject ();
                        if(dataObject.contains ("user"))
                        {
                            qDebug()<<"user"<<info.getmsg ();
                            QJsonValue userVal = dataObject.value ("user");
                            QJsonObject userObject = userVal.toObject ();
                            QJsonValue nameVal = userObject.take ("name");
                            name = nameVal.toString ();
                            info.setname (name);

                        }
                        qDebug()<<"发送信号"<<info.getmsg ();
                        emit readUserDone ();
                    }
                }
            }




        }

    }

}
