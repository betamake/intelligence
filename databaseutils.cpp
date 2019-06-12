#include "databaseutils.h"
#include "Information.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QSettings>
#include <QSqlError>
#include <QMessageBox>
#include<QSqlDriver>
#define IP     "127.0.0.1"
#define SQLUSER    "root"
#define SQLPASS    "123456"
#define DatabaseName  "mydata"


databaseUtils::databaseUtils()
{
    //    createConnectionByName("MyConnection");
    //    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "MyConnection");
    //    db.setHostName(IP);
    //    db.setDatabaseName(DatabaseName);
    //    db.setUserName(SQLUSER);
    //    db.setPassword(SQLPASS);

    //    if (!db.open()) {
    //        qDebug() << "Connect to MySql error: " << db.lastError().text();
    //        return;
    //    }
    //    qDebug() << "数据库连接成功"<<endl;
}

//创建连接
//void databaseUtils::createConnectionByName(const QString &connectionName) {

//}
//获取连接
//QSqlDatabase databaseUtils::getConnectionByName(const QString &connectionName) {
//    //    qDebug() << "获得数据库连接"<<endl;
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",connectionName);

//    if (QSqlDatabase::contains(connectionName)) {
//        db = QSqlDatabase::database(connectionName);
//    } else {
//        db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
//    }

//    return db;
//}

bool databaseUtils::verificationLogin(QString username, QString password)
{   
    User user = SearchUserByUsername(username);
    if(md5Encode(password) == user.getPassword())
    {
        qDebug()<<"该用户存在"<<endl;
        return true;
    }
    else
    {
        qDebug()<<"该用户不存在"<<endl;
        return false;
    }
}

//检查用户名是否存在
bool databaseUtils::checkUsername(QString username)
{
//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    query.exec("SELECT COUNT(*) FROM user where username ='"+username+"'");

    while (query.next()) {
        int count = query.value(0).toInt();
        qDebug()<<"count"<<count<<endl;
        if(count>0)
        {
            qDebug()<<"该用户存在"<<endl;
            return true;
        }else
        {
            return false;
        }

    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
* @brief         检查用户表中身份证是否存在
* @author        胡帅成
* @date        2018-09-11
*/
bool databaseUtils::CheckUserByIdCard(QString idCard)
{

    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    query.exec("SELECT COUNT(*) FROM user where id_card ='"+idCard+"'");

    while (query.next()) {
        int count = query.value(0).toInt();
        qDebug()<<"count"<<count<<endl;
        if(count>0)
        {
            qDebug()<<"身份证信息已存在"<<endl;
            return true;
        }else
        {
            return false;
        }

    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}



/**
 * @brief databaseUtils::outputUserInfo
 * 插入用户
 */
void databaseUtils::outputUserInfo() {

//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    query.exec("SELECT * FROM user");

    while (query.next()) {
        qDebug() << QString("uid: %1, username: %2, password: %3")
                    .arg(query.value("uid").toInt())
                    .arg(query.value("username").toString())
                    .arg(query.value("password").toString());
    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}


/**
 * @brief databaseUtils::insertUser
 * @param user
 * 插入用户
 */
bool databaseUtils::insertUser(User user)
{
    //    if(QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions))
    //    {
    //支持事务
    //        QSqlDatabase::database().transaction();
//    qDebug() << "插入数据";

//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);


    query.prepare("insert into mydata.user(uid,avater,autho_id,username,password,id_face,id_card,phone_number,create_time,update_time) values(?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(user.getUid());
    query.addBindValue(user.getAvater());
    query.addBindValue(user.getAuthoId());
    query.addBindValue(user.getUsername());
    query.addBindValue(md5Encode(user.getPassword()));
    query.addBindValue(user.getIdFace());
    query.addBindValue(user.getIdCard());
    query.addBindValue(user.getPhoneNumber());
    query.addBindValue(user.getCreateTime());
    query.addBindValue(user.getUpdateTime());
    bool result = query.exec();

    //        QSqlDatabase::database().commit();

    if(!result){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return false;
    }
    return true;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::insertIdCard
 * @param user
 * 插入用户
 */
bool databaseUtils::insertIdCard(Information information)
{

//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);


    //md5加密密码串  /*id_face,id_card,*/
    query.prepare("insert into id_card(id_number,name,sex,nation,birthday,address,signing_organization,validity_period_start,validity_period_end,newest_address) values(?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(QString::fromStdString(information.getIdNumber()));
    query.addBindValue(QString::fromStdString(information.getName()));
    query.addBindValue(QString::fromStdString(information.getSex()));
    query.addBindValue(QString::fromStdString(information.getNation()));
    query.addBindValue(QString::fromStdString(information.getBirthday()));
    query.addBindValue(QString::fromStdString(information.getAddress()));
    query.addBindValue(QString::fromStdString(information.getSigningOrganization()));
    query.addBindValue(QString::fromStdString(information.getValidityPeriodStart()));
    query.addBindValue(QString::fromStdString(information.getValidityPeriodEnd()));
    query.addBindValue(QString::fromStdString(information.getNewestAddress()));

    bool result = query.exec();
    if(!result){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return false;
    }
    return true;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}


/**
 * @brief databaseUtils::insertUser
 * @param user
 * 插入差旅报销单数据
 */
bool databaseUtils::insertBusiexp(busiExp busiexp)
{
    //    if(db.open())
    //    {
    //        qDebug() << "数据库连接成功"<<endl;
    //    }else
    //    {
    //        qDebug() << "数据库连接失败";
    //    }
    //    QSqlQuery query(db);
//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);


    //md5加密密码串  /*id_face,id_card,*/
    query.prepare("insert into busi_exp(busid,manid,abdid,begindate,enddate,day,relaxday,formadres,toadres,transcost,transbillnum,accomcost,accombillnum,transsubsidy,foodsubsidy,totalcost,othercost,othernum,othermemo) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(busiexp.getbusid());
    query.addBindValue(busiexp.getBusimanid());
    query.addBindValue(busiexp.getAbdid());
    query.addBindValue(busiexp.getBusibegindate());
    query.addBindValue(busiexp.getBusienddate());
    query.addBindValue(busiexp.getBusiday());
    query.addBindValue(busiexp.getBusirelaxday());
    query.addBindValue(busiexp.getBusifromadres());
    query.addBindValue(busiexp.getBusitoadres());
    query.addBindValue(busiexp.getBusitranscost());
    query.addBindValue(busiexp.getBusitransbillnum());
    query.addBindValue(busiexp.getBusiaccomcost());
    query.addBindValue(busiexp.getbusiAccombillnum());
    query.addBindValue(busiexp.getBusitranssubsidy());
    query.addBindValue(busiexp.getBusifoodsubsidy());
    query.addBindValue(busiexp.getBusitotalcost());
    query.addBindValue(busiexp.getBusiothercost());
    query.addBindValue(busiexp.getBusiothernum());
    query.addBindValue(busiexp.getBusiothermemo());
    bool result = query.exec();
    if(!result){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
    }
    return result;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::insertUser
 * @ljh
 * 插入出国报销单数据
 */

bool databaseUtils::insertAbdexp(abroadExp abdexp)
{
    //    if(db.open())
    //    {
    //        qDebug() << "数据库连接成功"<<endl;
    //    }else
    //    {
    //        qDebug() << "数据库连接失败";
    //    }
    //    QSqlQuery query(db);
//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);
    //md5加密密码串  /*id_face,id_card,*/
    query.prepare("insert into abroad_exp(abdid,visacost,visabillnum,servcost,servbillnum,exchgcost,exchgbillnum) values(?,?,?,?,?,?,?)");

    query.addBindValue(abdexp.getAbdid());
    query.addBindValue(abdexp.getAbdvisacost());
    query.addBindValue(abdexp.getAbdvisabillnum());
    query.addBindValue(abdexp.getAbdservcost());
    query.addBindValue(abdexp.getAbdservbillnum());
    query.addBindValue(abdexp.getAbdexchgcost());
    query.addBindValue(abdexp.getAbdexchgbillnum());

    bool result = query.exec();
    if(!result){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
    }
    return result;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);}

/**
 * @brief databaseUtils::insertUser
 * @ljh
 * 插入出国报销单数据
 */

bool databaseUtils::insertCostexp(costExp costexp)
{
//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    //md5加密密码串  /*id_face,id_card,*/
    query.prepare("insert into cost_exp(costid,manid,expensemoney,amount,budgetprojectid,centrializedid,loanid,surplushloan,memo) values(?,?,?,?,?,?,?,?,?)");

    query.addBindValue(costexp.getcostid());
    query.addBindValue(costexp.getCostmanid());
    query.addBindValue(costexp.getCostmoney());
    query.addBindValue(costexp.getCostbillamount());
    query.addBindValue(costexp.getCostbudgtproid());
    query.addBindValue(costexp.getCostcentrializedid());
    query.addBindValue(costexp.getCostloanid());
    query.addBindValue(costexp.getCostsurplusloan());
    query.addBindValue(costexp.getCostmemo());

    bool result = query.exec();
    if(!result){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
    }
    return result;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);}

/**
 * @brief databaseUtils::insertUser
 * @ljh
 * 插入表头信息数据
 */
bool databaseUtils::insertExpensehead(ExpenseHead exphead)
{

//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);
    //md5加密密码串  /*id_face,id_card,*/
    query.prepare("insert into exphead(busid,costid,deptid,operatorid,date,year,purpose,requisitionid,loanmoneysum,projectid,currency,exrate,einvoiceid,costtype,contractid,contractname) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

    query.addBindValue(exphead.getHeadbusid());
    query.addBindValue(exphead.getHeadcostid());
    query.addBindValue(exphead.getHeaddeptid());
    query.addBindValue(exphead.getHeadoperatorid());
    query.addBindValue(exphead.getHeadDate());
    query.addBindValue(exphead.getHeadyear());
    query.addBindValue(exphead.getHeadpurpose());
    query.addBindValue(exphead.getHeadrequisitionid());
    query.addBindValue(exphead.getHeadloanmoneysum());
    query.addBindValue(exphead.getHeadprojectid());
    query.addBindValue(exphead.getHeadcurrency());
    query.addBindValue(exphead.getHeadExrate());
    query.addBindValue(exphead.getHeadeinvoiceid());
    query.addBindValue(exphead.getHeadCosttype());
    query.addBindValue(exphead.getHeadcontractid());
    query.addBindValue(exphead.getHeadContractname());

    bool result = query.exec();
    if(!result){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
    }
    return result;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::insertUser
 * @ljh
 * 插入支付信息数据
 */
bool databaseUtils::insertPayinfo(PayInfo payinfo)
{

//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    //md5加密密码串  /*id_face,id_card,*/
    query.prepare("insert into pay_info(costid,busid,type,unit,bank,bankname,bankbum,bill,note) values(?,?,?,?,?,?,?,?,?)");

    query.addBindValue(payinfo.getPayInfoCostid());
    query.addBindValue(payinfo.getPayInfoBusid());
    query.addBindValue(payinfo.getPayinfoType());
    query.addBindValue(payinfo.getPayInfoUnit());
    query.addBindValue(payinfo.getPayInfoBank());
    query.addBindValue(payinfo.getPayInfoBankName());
    query.addBindValue(payinfo.getPayInfoBankNum());
    query.addBindValue(payinfo.getPayInfoBill());
    query.addBindValue(payinfo.getPayInfoNote());

    bool result = query.exec();
    if(!result){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
    }
    return result;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::insertUser
 * @ljh
 * 插入票据识别信息
 */
bool databaseUtils::insertBillcheck(BillCheck billinfo)
{
//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    //md5加密密码串  /*id_face,id_card,*/
    query.prepare("insert into bill_check(expenseid,busid,costid,attachtitle,realpath,billtype,content,code,number,user,startplace,endplace,date,money,checkcode,other) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

    query.addBindValue(billinfo.getBillexpenseid());
    query.addBindValue(billinfo.getBusid());
    query.addBindValue(billinfo.getCostid());
    query.addBindValue(billinfo.getBillattachmenttitle());
    query.addBindValue(billinfo.getBillrealpath());
    query.addBindValue(billinfo.getBilltype());
    query.addBindValue(billinfo.getBillcontent());
    query.addBindValue(billinfo.getBillcode());
    query.addBindValue(billinfo.getBillnumber());
    query.addBindValue(billinfo.getBilluser());
    query.addBindValue(billinfo.getBillstartplace());
    query.addBindValue(billinfo.getBillendplace());
    query.addBindValue(billinfo.getBilldate());
    query.addBindValue(billinfo.getBillmoney());
    query.addBindValue(billinfo.getBillcheckcode());
    query.addBindValue(billinfo.getBillother());

    bool result = query.exec();
    if(!result){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
    }
    return result;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

}

/**
 * @brief databaseUtils::md5Encode
 * @param string
 * 删除用户
 */
bool databaseUtils::deleteUser(QString username)
{

//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);


    bool res = query.exec("DELETE FROM user where username ='"+username+"'");
    //    qDebug()<<"res:"<<res<<endl;
    if (!res)
    {
        QSqlError lastError = query.lastError();
        qDebug()<<"Delete failed!"<<lastError.driverText() << lastError.databaseText()<<endl;
        return false;
    }
    return res;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::md5Encode
 * @param string
 * 修改用户信息
 */
bool databaseUtils::updateUser(User user)
{
    //    QSqlQuery query(db);
//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);


    bool res = query.exec("UPDATE mydata.user SET password='"+md5Encode(user.getPassword())
                          +"',autho_id='"+user.getAuthoId()
                          +"',id_face='"+user.getIdFace()
                          +"',id_card='"+user.getIdCard()
                          +"',phone_number='"+user.getPhoneNumber()
                          +"',update_time='"+user.getUpdateTime()
                          +"' where username ='"+user.getUsername()
                          +"'");
    if(!res)
    {
        QSqlError lastError = query.lastError();
        qDebug()<<"Update failed!"<<lastError.driverText() << lastError.databaseText()<<endl;
        return false;
    }
    return true;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::SearchUserByUsername
 * @param user
 * @return
 * 根据用户名查询账号
 */
User databaseUtils::SearchUserByUsername(QString username)
{
    User user;
    //    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    //肯定能查到
    query.exec("select * from mydata.user where username='"+username+"'");

    while (query.next()) {
        QString uid = query.value("uid").toString();
        QString authoId = query.value("autho_id").toString ();
        QVariant avater = query.value("avater").toByteArray();
        QString password = query.value("password").toString();
        QString id_face = query.value("id_face").toString();
        QString id_card = query.value("id_card").toString();
        QString phone_number = query.value("phone_number").toString();
        QString create_time = query.value("create_time").toString();
        QString update_time = query.value("update_time").toString();

        user.setUid(uid);
        user.setUsername(username);
        user.setAvater(avater);
        user.setAuthoId(authoId);
        user.setPassword(password);
        user.setIdCard(id_card);
        user.setIdFace(id_face);
        user.setPhoneNumber(phone_number);
        user.setCreateTime(create_time);
        user.setUpdateTime(update_time);
        return user;
    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::SearchUserByIdNumber
 * @param user
 * @return
 * 根据用户名查询账号
 */
User databaseUtils::SearchUserByIdNumber(QString idNumber)
{
    User user1;
//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    query.exec("select * from user where id_card='"+idNumber+"'");

    while (query.next()) {
        QString uid = query.value("uid").toString();
        qDebug()<<"uid:"<<uid;
        QString autho_id = query.value("autho_id").toString ();
        QString username = query.value("username").toString();
        QString password = query.value("password").toString();
        QString id_face = query.value("id_face").toString();
        QString id_card = query.value("id_card").toString();
        QString phone_number = query.value("phone_number").toString();
        QString create_time = query.value("create_time").toString();
        QString update_time = query.value("update_time").toString();

        user1.setUid(uid);
        user1.setAuthoId(autho_id);
        user1.setUsername(username);
        user1.setPassword(password);
        user1.setIdCard(id_card);
        user1.setIdFace(id_face);
        user1.setPhoneNumber(phone_number);
        user1.setCreateTime(create_time);
        user1.setUpdateTime(update_time);
        return user1;
    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}


/**
 * @brief databaseUtils::SearchUserByUsername
 * @param username
 * @return 根据log_id查询数据库
 */
User databaseUtils::SearchUserByUid(QString uid)
{
//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    User user;
    query.exec("select * from mydata.user where uid='"+uid+"'");

    while (query.next()) {
        QString autho_id = query.value("autho_id").toString ();
        QString username = query.value("username").toString();
        QString password = query.value("password").toString();
        QVariant avater = query.value("avater");
        QString id_face = query.value("id_face").toString();
        QString id_card = query.value("id_card").toString();
        QString phone_number = query.value("phone_number").toString();
        QString create_time = query.value("create_time").toString();
        QString update_time = query.value("update_time").toString();

        user.setUid(uid);
        user.setAuthoId(autho_id);
        user.setUsername(username);
        user.setPassword(password);
        user.setAvater(avater);
        user.setIdCard(id_card);
        user.setIdFace(id_face);
        user.setPhoneNumber(phone_number);
        user.setCreateTime(create_time);
        user.setUpdateTime(update_time);
        return user;
    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::CheckUserByLogId
 * @param username
 * @return 根据log_id查询数据库
 */
bool databaseUtils::CheckUserByUid(QString uid)
{

//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    query.exec("select count(*) from user where uid='"+uid+"'");

    while (query.next()) {
        int count = query.value(0).toInt();
        qDebug()<<"uid count："<<count<<endl;
        if(count>0)
        {
            qDebug()<<"该用户存在"<<endl;
            return true;
        }else
        {
            return false;
        }
    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/*
* @brief         检查身份证表中中身份证是否存在
* @author        胡帅成
* @date        2018-09-11
*/
bool databaseUtils::checkIdCargByIdNumber(QString idNumber)
{

//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    if (query.exec("SELECT COUNT(*) FROM mydata.id_card where id_number ='"+idNumber+"'"))
    {

        while (query.next()) {
            int count = query.value(0).toInt();
            qDebug()<<"count"<<count<<endl;
            if(count>0)
            {
                qDebug()<<"身份证信息已存在"<<endl;
                return true;
            }else
            {
                return false;
            }

        }
    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}


/**
 * @brief databaseUtils::SearchUserByIdNumber
 * @param user
 * @return
 * 根据用户名查询账号
 */
Information databaseUtils::searchIdCardByIdNumber(QString idNumber)
{

    Information information;
//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);
    query.exec("select * from mydata.id_card where id_number='"+idNumber+"'");

    while (query.next()) {
        string name = query.value("name").toString().toStdString();
        string sex = query.value("sex").toString().toStdString();
        string nation = query.value("nation").toString().toStdString();
        string birthday = query.value("birthday").toString().toStdString();
        string address = query.value("address").toString().toStdString();
        string signing_organization = query.value("signing_organization").toString().toStdString();
        string validity_period_start = query.value("validity_period_start").toString().toStdString();
        string validity_period_end = query.value("validity_period_end").toString().toStdString();
        string newest_address = query.value("newest_address").toString().toStdString();

        information.setName(name);
        information.setSex(sex);
        information.setNation(nation);
        information.setBirthday(birthday);
        information.setAddress(address);
        information.setSigningOrganization(signing_organization);
        information.setIdNumber(idNumber.toStdString());
        information.setValidityPeriodStart(validity_period_start);
        information.setValidityPeriodEnd(validity_period_end);
        information.setNewestAddress(newest_address);

        return information;
    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::md5Encode
 * @param string
 * 删除用户
 */
bool databaseUtils::deleteIdCard(QString idNumber)
{

//    QSqlQuery query(getConnectionByName("MyConnection"));
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

    bool res = query.exec("DELETE FROM mydata.id_card where id_number ='"+idNumber+"'");
    if (!res)
    {
        QSqlError lastError = query.lastError();
        qDebug()<<"Delete failed!"<<lastError.driverText() << lastError.databaseText()<<endl;
        return false;
    }
    return res;
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

/**
 * @brief databaseUtils::md5Encode
 * @param string
 * 修改用户信息
 */
bool databaseUtils::updateIdCard(Information information)
{
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);

//    QSqlQuery query(getConnectionByName("MyConnection"));

    bool res = query.exec("UPDATE mydata.id_card SET name='"+QString::fromStdString(information.getName())
                          +"',sex='"+QString::fromStdString(information.getSex())
                          +"',nation='"+QString::fromStdString(information.getNation())
                          +"',birthday='"+QString::fromStdString(information.getBirthday())
                          +"',address='"+QString::fromStdString(information.getAddress())
                          +"',signing_organization='"+QString::fromStdString(information.getSigningOrganization())
                          +"',validity_period_start='"+QString::fromStdString(information.getValidityPeriodStart())
                          +"',validity_period_end='"+QString::fromStdString(information.getValidityPeriodEnd())
                          +"',newest_address='"+QString::fromStdString(information.getNewestAddress())
                          +"' where id_number ='"+QString::fromStdString(information.getIdNumber())
                          +"'");
    if(!res)
    {
        QSqlError lastError = query.lastError();
        qDebug()<<"Update failed!"<<lastError.driverText() << lastError.databaseText()<<endl;
        return false;
    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
    return true;

}


/**
 * @brief databaseUtils::md5Encode
 * @param string
 * @return md5加密字符串
 */
QString databaseUtils::md5Encode(QString string)
{
    // generate MD5-Hash
    QByteArray md5_hash;
    md5_hash = QCryptographicHash::hash(string.toUtf8(), QCryptographicHash::Md5);
    QString md5hash_string = md5_hash.toHex();
    qDebug() << "MD5-Hash:" << md5hash_string;

    return md5hash_string;
}



