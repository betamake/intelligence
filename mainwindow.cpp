
//#include "winnt.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commonutils.h"

#include <QAudioProbe>
#include <QAudioRecorder>
#include <QMediaRecorder>
#include<QMediaPlayer>
#include<QCameraInfo>
#include<QCameraViewfinder>
#include<QCameraImageCapture>
#include<QCoreApplication>
#include<QFileDialog>
#include<QDebug>
#include <unistd.h>

#include <turing.h>

#include<QWidget>
#include <QQuickView>
#include <QTimer>
#include<QByteArray>
#include<QtNetwork>
#include<QMap>
#include<QFileDialog>
#include<QJsonDocument>
#include<QJsonParseError>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include<QScriptEngine>
#include<QScriptValue>
#include<QScriptValueIterator>
#include<QStandardItemModel>
#include<QMessageBox>
#include <QtGlobal>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDir>
#include <QString>
#include <QTime>
#include <QStringList>
#include <QThread>
#include <QDialog>
#include<QPainter>
#include<QRect>
#include<QVariant>
#include <QProcess>
#include <QDateTime>

static qreal getPeakValue(const QAudioFormat &format);
static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffr, int frames, int channels);


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),m_currentRunLoaclThread(NULL),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->initMedia();
    initObjects();
    ui->firstBtn->show ();
    currentIndex = 0;
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->faceManBtn->hide();//人脸库管理登陆后才展示
    ui->lastStepBtn->hide ();
//    ui->firstBtn->hide ();
    this->noShowguiInforn ();

    ui->userInfo->installEventFilter(this); //初始化个人信息页
    address = "http://192.168.3.185:8000";
    qDebug() << "length:"<< address.length();

    regCode = 0;//初始化注册信息
    loginStatus = 0;//未登录
    idCheckId = 0;//初始化身份证调用id

    isScan=false;

    billList.clear();

    qRegisterMetaType<Information>("Information");//注册information类
    qRegisterMetaType<QMap<QString,QString>>("QMap<QString,QString>");//注册information类
    qRegisterMetaType<QByteArray>("QByteArray");//注册QByteArray类

    totalReimAccount = 0;

    mPayItemList.clear();
    totalPayAccount = 0;

    mPersonType = 1;
    totalPerAccount = 0;

    expenseType = 11;//报销类型页面号   //这两行没有用 @hkl
    expenseTypeId = 11;//判断提交数据的前一页

    //

}

MainWindow::~MainWindow()
{
    qDebug() << "start destroy widget";
    delete ui;
    qDebug() << "end destroy widget";
}

//初始化控件
void MainWindow::initObjects()
{
    //costBaseinfo
    ui->costRnumEdit->setFocusPolicy(Qt::NoFocus);      //单据编号不允许修改
    ui->costRdateEdit->setFocusPolicy(Qt::NoFocus);     //单据日期不允许修改
    ui->costHandpEdit->setFocusPolicy(Qt::NoFocus);     //经办人不允许修改
    ui->costHandpdEdit->setFocusPolicy(Qt::NoFocus);    //经办人部门

    //baseinfo
    ui->docuNumber->setFocusPolicy(Qt::NoFocus);        //单据编号不允许修改
    ui->duckDate->setFocusPolicy(Qt::NoFocus);          //单据日期不允许修改
    ui->transactor->setFocusPolicy(Qt::NoFocus);        //经办人
    ui->tranDepartment->setFocusPolicy(Qt::NoFocus);    //经办人部门

    //
}
//QString MainWindow::rsa_pub_encrypt_base64 (const QString strClearData, const QString PubKey)
//{
//    QByteArray pubKeyArry = strPubKey.toUtf8();
//    uchar* pPubKey = (uchar*)pubKeyArry.data();
//    BIO* pKeyBio = BIO_new_mem_buf(pPubKey, pubKeyArry.length());
//    if (pKeyBio == NULL){
//        return "";
//    }
//    RSA* pRsa = RSA_new();
//    if ( strPubKey.contains(BEGIN_RSA_PUBLIC_KEY) ){
//        pRsa = PEM_read_bio_RSAPublicKey(pKeyBio, &pRsa, NULL, NULL);
//    }else{
//        pRsa = PEM_read_bio_RSA_PUBKEY(pKeyBio, &pRsa, NULL, NULL);
//    }
//    if ( pRsa == NULL ){
//        BIO_free_all(pKeyBio);
//        return "";
//    }

//    int nLen = RSA_size(pRsa);
//    char* pEncryptBuf = new char[nLen];
//    memset(pEncryptBuf, 0, nLen);

//    QByteArray clearDataArry = strClearData.toUtf8();
//    int nClearDataLen = clearDataArry.length();
//    uchar* pClearData = (uchar*)clearDataArry.data();
//    int nSize = RSA_public_encrypt(nClearDataLen,
//                                   pClearData,
//                                   (uchar*)pEncryptBuf,
//                                   pRsa,
//                                   RSA_PKCS1_PADDING);
//    QString strEncryptData = "";
//    if ( nSize >= 0 ){
//        QByteArray arry(pEncryptBuf, nSize);
//        strEncryptData = arry.toBase64();
//    }
//    // 释放内存
//    delete pEncryptBuf;
//    BIO_free_all(pKeyBio);
//    RSA_free(pRsa);
//    return strEncryptData;
//}
//初始化多媒体
void MainWindow::initMedia()
{
    //语音识别设置
    audioRecorder = new QAudioRecorder(this);
    probe = new QAudioProbe;

    connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)),this, SLOT(processBuffer(QAudioBuffer)));
    probe->setSource(audioRecorder);
    recordNum = 0;
    recordHigh = 0;
    recordLow = 0;
    recordSend = false;
    this->setRecord();
    this->startRecord();

    //初始化音频播放变量
    player = new QMediaPlayer;
    playStatus = false;
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(playChanged()));

    player->stop();
    this->sendPlayText("欢迎使用财务机器人");
}

//-------------------------------------------event start----------------------------------------
//首页登录方式选择
void MainWindow::on_LogBtn_clicked()
{
    player->stop();
    this->sendPlayText("请选择登录方式");
    currentIndex =2;
    ui->mainWidget->setCurrentIndex(currentIndex);
}


/**
* @brief      登录方式选择人脸
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::on_faceBtn_clicked()
{
    player->stop();
    this->sendPlayText("已选择人脸登录");
    qDebug()<<"人脸登录"<<CameraDevice::getinstance ()->CameraInfo.getviewfinder ();
    ui->imageLayout->addWidget(CameraDevice::getinstance ()->CameraInfo.getviewfinder ());
    CameraDevice::getinstance ()->CameraInfo.getcamera ()->start();
    CameraDevice::getinstance ()->moveToThread (CameraDevice::getinstance ()); //解决类不在一个线程
    CameraDevice::getinstance ()->start ();
    // jump to facecheck page
    currentIndex = 3;
    ui->mainWidget->setCurrentIndex(currentIndex);

     connect (CameraDevice::getinstance (),SIGNAL(faceCheckDone(int)),this,SLOT(dealFaceCheckDone(int)));
    connect (CameraDevice::getinstance (),SIGNAL(faceCheckFailure(int)),this,SLOT(dealFaceCheckDone(int)));
}
void MainWindow::dealFaceCheckDone (int sendIndex)
{
    CameraDevice::getinstance ()->quit ();
    CameraDevice::getinstance ()->wait ();
    currentIndex=sendIndex;
    ui->mainWidget->setCurrentIndex(currentIndex);
}
void MainWindow::dealFaceCheckFailure (int sendIndex)
{
    CameraDevice::getinstance ()->quit ();
    CameraDevice::getinstance ()->wait ();
    currentIndex=sendIndex;
    ui->mainWidget->setCurrentIndex(currentIndex);
}
void MainWindow::on_RegAcountBtn_clicked()
{
    allInterface::getinstance ()->info.setusername (ui->RegUsername_LineEdit->text ());
    allInterface::getinstance ()->info.setpassword (ui->RegPwd_LineEdit->text ());
    currentIndex =7;
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->faceRegImageLayout->addWidget(faceReg::getinstance ()->CameraInfo.getviewfinder ());
    faceReg::getinstance ()->CameraInfo.getcamera ()->start();
    faceReg::getinstance ()->moveToThread (faceReg::getinstance ()); //解决类不在一个线程
    faceReg::getinstance ()->start ();
    connect(faceReg::getinstance (),SIGNAL(faceRegSucess(int)),this,SLOT(dealFaceRegSucess(int)));
    connect(faceReg::getinstance (),SIGNAL(faceRegFailure(int)),this,SLOT(dealFaceRegFailure(int)));
}
void MainWindow::dealFaceRegSucess (int sendIndex)
{
    this->on_faceBtn_clicked();
    faceReg::getinstance ()->quit ();
    faceReg::getinstance ()->wait ();
}
void MainWindow::dealFaceRegFailure (int sendIndex)
{
    currentIndex=6;
    ui->mainWidget->setCurrentIndex (currentIndex);
    faceReg::getinstance ()->quit ();
    faceReg::getinstance ()->wait ();
}

/**
* @brief        身份证登录
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::on_idBtn_clicked()
{
    player->stop();
    this->sendPlayText("请将身份证放在指定位置");
    currentIndex =5;
    //进去之前先隐藏控件
    ui->idUsername->setAlignment(Qt::AlignCenter);
    ui->idUsername->setText(tr("正在识别中...."));
    ui->idLabel->hide();
    ui->idLabel2->hide();
    ui->idLabel3->hide();
    ui->idLabel4->hide();
    ui->idLabel5->hide();

    //隐藏显示框
    ui->idName->hide();
    ui->idAddress->hide();
    ui->idCardNumber->hide();
    ui->idPhone->hide();
    ui->idSex->hide();

    idCheckId = 2;
    isIdcardChecked = false;
    ui->mainWidget->setCurrentIndex(currentIndex);
    this->idCardRead();
}
/**
* @brief         身份证登录查询数据库
* @author        胡帅成
* @date        2018-09-08
*/
void MainWindow::on_idLogin_clicked()
{
    //走到这里必然登录成功了
    if(isIdcardChecked)
    {
        player->stop();
        this->sendPlayText("身份证登录成功");
//        ui->lastStepBtn->show ();
//        ui->nextStepBtn->show ();
        currentIndex = 9;
        ui->mainWidget->setCurrentIndex(currentIndex);//登录成功跳转到欢迎页
        ui->lastStepBtn->hide ();
        ui->nextStepBtn->hide ();
        //登录完成展示用户信息
        loginStatus = 2;
        this->showUserInfo(loginUser);

    }else
    {
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("请将身份证放到指定位置!"));
        player->stop();
        this->sendPlayText("请将身份证放到指定位置");
    }

}

/**
* @brief        选择账号
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::on_accountBtn_clicked()
{
    player->stop();
    currentIndex = 4;
    this->sendPlayText("已选择账号登录");
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->lastStepBtn->show ();
}
//账户登录操作
void MainWindow::on_user_loginBtn_clicked()
{
    ui->lastStepBtn->show ();//显示上一步按钮
    ui->nextStepBtn->show ();
    QString username = ui->user_loginEdit->text().trimmed();
    QString password = ui->user_pwdEdit->text().trimmed();
    if(!(username.isEmpty() && password.isEmpty())){
       allInterface::getinstance ()->info.setusername (username);
       allInterface::getinstance ()->info.setpassword (password);
       allInterface::getinstance ()->userInterface ();
       connect (allInterface::getinstance (),SIGNAL(readUserDone()),this,SLOT(userLogin()));
    }else
    {
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("用户名密码不能为空！"));
        player->stop();
        this->sendPlayText("用户名密码不能为空");
    }

}

void MainWindow::userLogin ()
{
    if (allInterface::getinstance ()->info.getmsg ()=="登录成功")
    {
        player->stop();
        this->sendPlayText("账号登录成功");
        currentIndex = 9;
        ui->mainWidget->setCurrentIndex (currentIndex);
        ui->lastStepBtn->hide ();
        ui->nextStepBtn->hide ();
        ui->user_loginEdit->clear();
        ui->user_pwdEdit->clear();
        ui->user_loginEdit->setFocus();
    }
    else
    {
        qDebug()<<"获取msg信息:"<<endl;
        //登录失败，清空用户编辑框，密码编辑框，设置光标到用户编辑框
        QMessageBox::information(this, QString::fromUtf8("失败"),QString::fromUtf8("登录失败！"));
        player->stop();
        this->sendPlayText("账号有误，请重新登录");
        //清空内容并定位光标
        ui->user_loginEdit->clear();
        ui->user_pwdEdit->clear();
        ui->user_loginEdit->setFocus();
    }

}

//首页注册方式选择
void MainWindow::on_RegBtn_2_clicked()
{
    currentIndex = 6;
    ui->mainWidget->setCurrentIndex(currentIndex);


}




/**
* @brief      发送音频文本，进行语音合成
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::sendPlayText(QByteArray text){
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(playReply(QNetworkReply*)));

    QUrlQuery params;

    text = text.toBase64();
    text = text.replace("+","-");
    text = text.replace("/","_");
    params.addQueryItem("text",text);
    QString  data = params.toString();

    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/understand/synthesis/"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->post(request,params.toString().toUtf8());
}


/**
* @brief         票据识别事件
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::on_fileChooseBtn_clicked()
{

    player->stop();
    this->sendPlayText("请选择票据");
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("打开图片"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg)"));
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(fileDialog->exec()){
        fileNames = fileDialog->selectedFiles();
    }
    QString fileName;
    fileName = fileNames.at(0);
    billpath = fileName;
    qDebug()<<fileName<<endl;
    manager = new QNetworkAccessManager(this);
    //读取完成后识别票据
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(billReply(QNetworkReply*)));

    QFile file(fileName);
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

    QUrlQuery params;
    params.addQueryItem("image",fdata);
    params.addQueryItem("imageType",".jpg");
    QString  data = params.toString();

    //打印票据图片编码
    this->printFile("billReply.txt", fdata);
    //    QString address = HOST.append("/bill/");

    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/bill/"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->post(request,params.toString().toUtf8());
}


void MainWindow::toLastStep(){
    //报销类型的上一步是页面
    if(currentIndex == 17){
        currentIndex = 15;
    }
    else if(currentIndex == 15){
        if (expenseType==12||expenseType==14)
            currentIndex = 16;
        else if(expenseType==11)
            currentIndex=14;
    }
    else if(currentIndex == 14 || currentIndex == 16 ){
        currentIndex = 13;
    }
    else if(currentIndex == 13){
        if (expenseType == 11)
            currentIndex = 11;
        else if (expenseType == 12 || expenseType == 14)
            currentIndex = 12;
        }
    else if (currentIndex==11 || currentIndex==12)
    {
        ui->lastStepBtn->hide();
        currentIndex=10;
    }
    else if (currentIndex==10)
    {
        currentIndex=9;
    }
    else if (currentIndex==3 || currentIndex==4 ||currentIndex==5 ||currentIndex==6)//登录方式
    {
        currentIndex=2;
    }
    else if (currentIndex==2)
    {
        currentIndex=1;
    }
    else if(currentIndex > 0 && currentIndex != 9){
        currentIndex -- ;
    }
    ui->mainWidget->setCurrentIndex(currentIndex);
}
/*
 * currentIndex：
   0:welcomepage  //三个模块选择
   1:startpage  用户设置  登录还是注册// 显示上一步
   2:loginpage  登录方式 // 显示上一步

   3:face 人脸登录  //上一步
   4:acount 账户登录  //上一步
   5:idCord 身份证登录  //上一步

   6:accountReg:账号注册 //上一步
   7:faceReg 人脸注册 //上一步
   8:RegChoose 身份证认证 //上一步

   9:begin:即将开始报销页 //上一步 下一步
   10:expenseChoice 报销类别 //上一步

   11:costBaseInfo  费用报销基本信息 //上下,下一步要先确认已经填完内容，结束了本次报销的时候清除该页的信息
   12:baseInfo 差旅和出国报销明细 //上下,下一步要先确认已经填完内容，结束了本次报销的时候清除该页的信息
   13:bill 票据录入 //上下,结束了本次报销的时候清除该页的信息
   14:costItem  费用报销明细 // 上下,结束了本次报销的时候清除该页的信息
   15:payinfo 支付信息 // 上下,结束了本次报销的时候清除该页的信息
   16:addPersonnelWidget 添加人员信息	//上下，结束了本次报销的时候清除该页的信息
   17:finish 报销完成
*/
//下一步
void MainWindow::toNextStep(){
    //选择身份验证的下一步是3
    if(currentIndex == 3 || currentIndex == 4 || currentIndex == 5){//在人脸识别或用户名密码登陆或身份证识别页面
        currentIndex = 9;//跳转到选择报销类别页面
    }
//    else if(currentIndex == 7 || currentIndex == 8) {
//        //ui->nextStepBtn->hide();
//    }
//    else if(currentIndex == 6){//注册成功的下一页是登录页1
//        currentIndex ++;
//    }
    else if(currentIndex == 6 || currentIndex == 7 || currentIndex == 8){//注册成功后跳到开始报销页面
//        currentIndex = 9;
    }
    else if(currentIndex == 9){//欢迎页
        currentIndex =10;
        ui->lastStepBtn->hide ();
        ui->nextStepBtn->hide ();
    }
    else if(currentIndex ==11 )
    {
        bool finished = true;
        QString str = "请填写：";

        //由于还没有结合接口导入单据编号，人员信息，省略这三个必填项的判断
        if(ui->costRnumEdit->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("单据编号");
//            postData.getCostData ().setcode (ui->costRnumEdit->text().toInt ());
        }
        if(ui->costHandpEdit->text().isEmpty()) {
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("经办人");
        }
        if(ui->costHandpdEdit->text().isEmpty()){
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("经办人部门");
        }
        if(ui->costUseEdit->text().isEmpty()){
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("用途");
        }
        if (!finished){
            QMessageBox::warning(this, "warning", str, QMessageBox::Ok);
        } else {
            currentIndex =  13;
            ui->mainWidget->setCurrentIndex (currentIndex);
            this->costbaseRead ();
        }
    }
    else if(currentIndex ==12)
    {
        bool finished = true;
        QString str = "请填写：";
//        QString unFixedStr = "";
        //录入票据之后需要点击下一步需要进行判定  结合标志位

        //由于还没有结合接口导入单据编号，人员信息，省略这三个必填项的判断
//        if(ui->docuNumber->text().isEmpty()) {
//            finished = false;
//            str = QString("%1 %2 ").arg(str).arg("单据编号");
//        }
//        if(ui->transactor->text().isEmpty()) {
//            finished = false;
//            str = QString("%1 %2 ").arg(str).arg("经办人");
//        }
//        if(ui->tranDepartment->text().isEmpty()){
//            finished = false;
//            str = QString("%1 %2 ").arg(str).arg("经办人部门");
//        }
        if(ui->busiReason->toPlainText().isEmpty()){
            finished = false;
            str = QString("%1 %2 ").arg(str).arg("出差事由");
        }
        if(!finished)
            QMessageBox::warning(this, "warning", str, QMessageBox::Ok);
        else {
            currentIndex=13;
            ui->mainWidget->setCurrentIndex (currentIndex);
        }
    }
    else if(currentIndex == 13){
        if (expenseType == 11)
            currentIndex=14;
        else if (expenseType == 12 || expenseType == 14)
            currentIndex=16;
        ui->mainWidget->setCurrentIndex (currentIndex);

    }
    else if(currentIndex == 14){
        if (mReimItemList.size() == 0)
            QMessageBox::warning(this, "waring", "报销明细至少有一条！", QMessageBox::Ok);
        else {
//            emit saveReimDetail();
            addReimAccount();
            //需要确认把所有的条目加进去再换到下页，还需要修改一下
            currentIndex=15;
            ui->mainWidget->setCurrentIndex (currentIndex);
        }
    }
    else if(currentIndex == 15){
        currentIndex=17;
        ui->mainWidget->setCurrentIndex (currentIndex);
    }
    else if(currentIndex == 16){
        currentIndex=15;
        ui->mainWidget->setCurrentIndex (currentIndex);
    }
    else if(currentIndex == 16){
        //必须有一条人员信息
        if (ui->personnelList->count() == 0)
            QMessageBox::warning(this, "warning", "必须有一条人员信息", QMessageBox::Ok);
        else {
            //保存退出
            currentIndex=15;
            ui->mainWidget->setCurrentIndex (currentIndex);
        }
    }
}


void MainWindow::toCurrentPage (int pageNum)
{
    ui->mainWidget->setCurrentIndex (pageNum);
    switch (pageNum) {
    case 0:
        break;
    case 1:
        ui->firstBtn->show ();
        ui->lastStepBtn->show();

        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        //the next step we should init a carma
        //设置取景器
//        viewfinder = new QCameraViewfinder(this);
//        ui->faceRegImageLayout->addWidget(viewfinder);
//        camera->setViewfinder(viewfinder);
//        viewfinder->resize(600,400);

//        imageCaptureReg = new QCameraImageCapture(camera);
//        camera->setCaptureMode(QCamera::CaptureStillImage);
//        imageCaptureReg->setCaptureDestination(QCameraImageCapture::CaptureToFile);

//        camera->start();

        //跳到人脸注册页面
        currentIndex = 6;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    case 7:
        //the next step ,wes should check the idcard
        if(m_currentRunLoaclThread)
        {
            m_currentRunLoaclThread->stopImmediately();
        }
        this->idCardRead();
        currentIndex=8;
        ui->mainWidget->setCurrentIndex (currentIndex);

        //clear idcard input box
        ui->idName_2->setText("");
        ui->idBirth_2->setText("");
        ui->idCardNum_2->setText("");
        ui->idHomeAddre_2->setText("");
        ui->idSex_2->setText("");
        ui->idNation_2->setText("");
        ui->idProvePlace_2->setText("");
        ui->idStartDay_2->setText("");
        ui->idEndDay_2->setText("");
        ui->idNewestAddre_2->setText("");
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 12:
        break;
    case 13:
        break;
    case 14:
        break;
    case 15:
        break;
    case 16:
        break;
    case 17:
        break;
    default:
        break;
    }
}


void MainWindow::on_lastStepBtn_clicked()
{
    player->stop();
    this->sendPlayText("上一步");

    qDebug() <<"当前页号"<<currentIndex;
    switch (currentIndex) {
    case 3:
        this->deleteFace ();
        break;
    case 5:
        this->idCardThreadStop ();
        break;
    case 4:
        this->deleteAccountInfo ();
        break;
    default:
        break;
    }
    this->toLastStep();
}

void MainWindow::on_nextStepBtn_clicked()
{
    player->stop();
    this->sendPlayText("下一步");
    qDebug()<<currentIndex<<"111111111";
    this->toNextStep();
}
/*
 * currentIndex：
   0:welcomepage  //两个模块选择，不显示上下步
   1:startpage  用户设置  登录还是注册// 显示上一步
   2:loginpage  登录方式 // 显示上一步
   3:face 人脸登录  // 上一步
   4:acount 账户登录  //上一步
   5:idCord 身份证登录  //上一步
   6:accountReg 帐号注册 //上一步
   7:faceReg 人脸注册 //上一步
   8:idCardReg:身份证注册 //上一步
   9:begin:即将开始报销页 //上一步
   10:expenseChoice 报销类别 //上一步
   11:costBaseInfo  费用报销基本信息 //下，下一步要先确认已经填完内容，结束了本次报销的时候清除该页的信息
   12:baseInfo 差旅和出国报销明细 //下，下一步要先确认已经填完内容，结束了本次报销的时候清除该页的信息
   13:bill 票据录入 //上下，结束了本次报销的时候清除该页的信息
   14:costItem  费用报销明细 // 上下，结束了本次报销的时候清除该页的信息
   15:payinfo 支付信息 // 上下，结束了本次报销的时候清除该页的信息
   16:otherItem 其他事项	//上下，下一页是15,结束了本次报销的时候清除该页的信息
   17:finish 报销完成
*/
//当前页发生变化调用函数
void MainWindow::on_mainWidget_currentChanged(int arg1)
{
    ui->lastStepBtn->hide();
    ui->nextStepBtn->hide();

    //1-10,13-16显示上一步按钮
    if(currentIndex >0 && currentIndex <11){
        ui->lastStepBtn->show();
    }
    if(currentIndex >12 && currentIndex < 17){
        ui->lastStepBtn->show();
    }

    //11-16显示下一步按钮
    if(currentIndex > 10 && currentIndex < 17){
        ui->nextStepBtn->show();
    }

}
//选择差旅报销
void MainWindow::on_busiBtn_clicked()
{
    player->stop();
    this->sendPlayText("已选择差旅报销");
    QDateTime local(QDateTime::currentDateTime());
    ui->duckDate->setText (local.toString("yyyy-MM-dd"));
    ui->transactor->setText (allInterface::getinstance ()->info.getname ());
    ui->tranDepartment->setText (allInterface::getinstance ()->info.getofficeName ());

    chargeManName = allInterface::getinstance ()->info.getname ();
    chargeDepartment = allInterface::getinstance ()->info.getofficeName ();

    expenseType = 12;
    mPersonType = 1;        //添加的人员类型是1，差旅人员
    mTravelPersons.clear();
    setBasePage(12);
    ui->personnelList->clear();
}

//选择费用报销单
void MainWindow::on_costBtn_clicked()
{
    player->stop();
    this->sendPlayText("已选择费用报销");
    QDateTime local(QDateTime::currentDateTime());
    expenseType = 11;
    currentIndex = 11;
    ui->costHandpEdit->setText (allInterface::getinstance ()->info.getname ());
    ui->costHandpdEdit->setText (allInterface::getinstance ()->info.getofficeName ());
    ui->costRdateEdit->setText (local.toString("yyyy-MM-dd"));
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->nextStepBtn->show ();
    //显示报销流程按钮
    this->isShowguiInforn ();

    chargeManName = allInterface::getinstance ()->info.getname ();
    chargeDepartment = allInterface::getinstance ()->info.getofficeName ();

    mReimItemList.clear();
    ui->feeDetailList->clear();
    on_addDetailBtn_clicked();
}


//选择出国保险单
void MainWindow::on_abroadBtn_clicked()
{
    player->stop();
    this->sendPlayText("已选择出国报销");
    QDateTime local(QDateTime::currentDateTime());
    ui->duckDate->setText (local.toString("yyyy-MM-dd"));
    ui->transactor->setText (allInterface::getinstance ()->info.getname ());
    ui->tranDepartment->setText (allInterface::getinstance ()->info.getofficeName ());

    chargeManName = allInterface::getinstance ()->info.getname ();
    chargeDepartment = allInterface::getinstance ()->info.getofficeName ();

    expenseType = 14;
    mPersonType = 2;        //添加的人员类型是2,出国人员
    mAbroadPersons.clear();
    setBasePage(13);
    ui->personnelList->clear();
}

/**
* @brief         选择是差旅报报销还是出国报销，两者不一样
* @author        黄梦君
* @date          2019-06-22
*/
void MainWindow::setBasePage(int expenseType)
{
    //差旅报销
    if (expenseType == 12) {

    }
    else if (expenseType == 13) {

    }
    else
        return;

    currentIndex = 12;
    ui->mainWidget->setCurrentIndex(currentIndex);
    ui->nextStepBtn->show ();
    //显示报销流程按钮
    this->isShowguiInforn ();
}




//继续报销
void MainWindow::on_continueExpenseBtn_clicked()
{
    player->stop();
    this->sendPlayText("继续报销");

    clearAllInput();

    costRowcount= 1; //费用报销单rowcount为1
    //clear table
    // busi.init ();
    currentIndex = 10;
    ui->mainWidget->setCurrentIndex (currentIndex);

}
/**
* @brief         清空登录信息
* @author        胡坤伦
* @date        2019-03-11
*/
void MainWindow::deleteAccountInfo ()
{
    ui->user_loginEdit->clear ();
    ui->user_pwdEdit->clear ();
}
/**
* @brief         清空注册信息
* @author        胡坤伦
* @date        2019-03-11
*/
void MainWindow::deleteAccountReg ()
{
    ui->RegPwd_LineEdit->clear ();
    ui->RegUsername_LineEdit->clear ();
}

/**
* @brief         回收人脸识别资源
* @author        胡坤伦
* @date        2019-03-11
*/
void MainWindow::deleteFace ()
{
    qDebug() << "销毁人脸登录";
//    camera->stop();
//    if(timer->isActive ())
//    {
//        timer->stop ();
//    }
    CameraDevice::getinstance ()->CameraInfo.getcamera ()->stop ();
    CameraDevice::getinstance ()->quit ();
    CameraDevice::getinstance ()->wait ();


}



//结束报销
void MainWindow::on_logoutBtn_clicked()
{
    player->stop();
    this->sendPlayText("退出");

    clearAllInput();

    currentIndex =0;
    ui->mainWidget->setCurrentIndex (currentIndex);
    expenseType = 9;
    ui->userInfo->hide();
}

//-------------------------------------------sequence action end----------------------------------------



//-------------------------------------------slots start----------------------------------------
/**
* @function   photoRegister
* @brief      人脸注册结果处理
* @author        胡想成
* @date          2018-08-17
*/
void MainWindow::photoRegister(int id,QImage image)
{
    if(idFace==1){
        QByteArray fdata = this->getPixmapData("/files/regFace/",image);
        camera->stop();//停止相机拍照

        //准备发送网络请求和接收应答
        manager = new QNetworkAccessManager(this);
        QUrlQuery params;
        //不能直接到第二步
        QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceRegReply(QNetworkReply*)));

        fdata = fdata.toBase64();
        fdata = fdata.replace("+","-");
        fdata = fdata.replace("/","_");
        params.addQueryItem("image",fdata);
        params.addQueryItem("group_id","1");
        params.addQueryItem("uid",regUser.getUid());
        params.addQueryItem("user_info",allInterface::getinstance ()->info.getusername ());
        params.addQueryItem("imageType",".jpg");

        QString  data = params.toString();

        //打印请求参数(测试)
        this->printFile("reply.txt", fdata);
        QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/register/"));
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
        manager->post(request,params.toString().toUtf8());
    }
}

/**
* @brief       人脸注册结果处理
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::faceRegReply(QNetworkReply *reply){

    //拿到返回结果
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
            if(doucment.isObject()){  // JSON 文档为对象
                QJsonObject object = doucment.object();  //  转化为对象
                qDebug()<< "----faceReg result:----"<<QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","") <<endl;
                /*
                    注册成功
                    {
                        "log_id": 2764578038040222
                    }
                    注册失败

                    {
                        "error_code": 216616,
                        "log_id": 674786177,
                        "error_msg": "image exist"
                    }
                 */
                QString error_msg="";

                //注册失败
                if(object.contains("error_code")){
                    QJsonValue numValue = object.value("error_code");
                    //                    QJsonValue numValue2 = object.value("error_msg");
                    if(numValue.isDouble()){
                        //                        error_code = numValue.toInt();
                    }
                    qDebug()<< "----result failure:----"<< error_msg<<endl;
                    QMessageBox::information(this, QString::fromUtf8("警告!"),QString::fromUtf8("人脸注册失败!"));

                    toCurrentPage (6);
                }
                //注册成功
                else{
                    //取log_id  16位字符串
                    log_id =  QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","").mid(8,16);
                    qDebug() <<"log_id:"<<log_id<<endl;
                    qDebug() <<"user:"<< regUser.toString()<<endl;

                    //注册成功插入数据库
                    if(database.CheckUserByUid(regUser.getUid()))
                    {
                        player->stop();
                        this->sendPlayText("人脸注册成功");
                        QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("人脸注册成功！"));
                        //认证身份证
                        idCheckId = 1;//注册的时候调用
                        regCode = 2;//人脸注册完成
                        toCurrentPage (7);
                    }
                    else
                    {
                        player->stop();
                        this->sendPlayText("账户注册失败");
                        regCode = 0;
                        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("注册失败"));

                        toCurrentPage (6);
                    }
                }
                //清空内容并定位光标
                ui->RegUsername_LineEdit->clear();
                ui->RegPwd_LineEdit->clear();
            }
        }
    }
}


/**
* @function   faceCheck
* @brief        检测人脸库登录
* @author        胡想成
* @date          2018-08-17
*/
void MainWindow::faceCheck(){
    if(QCamera::ActiveStatus == camera->status())
    {
        qDebug()<<"第" << facetime << "次查询人脸";
        facetime++;
        idFace = 2;//登录查人脸

        //take photos
//        QCoreApplication::processEvents ();
        imageCapture->capture();
        if(facetime < 2)
        {
            connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(sendPhoto(int,QImage)));
        qDebug() << "face check success,111111111"<< endl;

        }else
        {
            qDebug() << "face check failed, stop send photos"<< endl;
            QMessageBox::information(this, QString::fromUtf8("失败"),QString::fromUtf8("人脸不存在，请补录人脸信息"));
            if(timer->isActive ())
            {
                timer->stop ();
            }
            camera->stop ();
//            currentIndex =6;
//            ui->mainWidget->setCurrentIndex(currentIndex);
            toCurrentPage (6);
        }

    }
}
/**
* @function   faceReply
* @brief       人脸登录结果处理
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::faceReply(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError) && doucment.isObject()) {

            QJsonObject object = doucment.object();
            //            qDebug()<< QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","")<<endl;
            //获取result_num参数
            int result_num = 0;
            if(object.contains("result_num"))
            {
                QJsonValue numValue = object.value("result_num");
                if(numValue.isDouble()){
                    result_num = numValue.toInt();
                }
            }
            if(result_num >=1 && object.contains("result"))
            {//人脸识别有结果
                QJsonValue resultsValue = object.value("result");
                if(resultsValue.isArray())
                {
                    QJsonArray resultsArray = resultsValue.toArray();
                    qDebug() << "resultsArray:" << resultsArray;

                    QString uid;
                    QString user_info;
                    double score = 80;//阈值为80分
                    //                    int nSize = resultsArray.size();
                    for(int i = 0  ; i <  resultsArray.size(); i ++){
                        QJsonValue resultValue = resultsArray.at(i);
                        double scoreTemp = 0;
                        if(resultValue.isObject()){
                            QJsonObject resultObj = resultValue.toObject();
                            //获取相似度分数
                            if(resultObj.contains("scores")){
                                QJsonValue scoresVal = resultObj.value("scores");
                                if(scoresVal.isArray()){
                                    QJsonArray scoresArray = scoresVal.toArray();
                                    QJsonValue scoreVal = scoresArray.at(0);
                                    if(scoreVal.isDouble()){
                                        scoreTemp = scoreVal.toDouble();
                                        qDebug() << "scoreTemp:" << scoreTemp;
                                    }
                                }
                            }
                            if(scoreTemp > score){
                                score = scoreTemp;
                                //获取唯一uid
                                if(resultObj.contains("uid")){
                                    QJsonValue uidVal = resultObj.value("uid");
                                    if(uidVal.isString()){
                                        uid = uidVal.toString();
                                    }
                                }
                                // 获取user_info
                                if(resultObj.contains("user_info")){
                                    QJsonValue infoVal = resultObj.value("user_info");
                                    if(infoVal.isString()){
                                        user_info = infoVal.toString();
                                    }
                                }
                            }
                        }
                    }
                    if(score > 80){
                        //相似度分数大于80,识别成功
                        qDebug() << "user_info:" << user_info;
                        qDebug() << "uid:" << uid;
                        uname = user_info;
                        isFaceOk = true;
                        loginStatus = 1;//人脸已登录
                        player->stop();
                        this->sendPlayText("登录成功");

                        camera->stop();
                        if(timer->isActive ())
                        {
                            timer->stop ();
                        }
                        currentIndex = 9;
                        ui->mainWidget->setCurrentIndex(currentIndex);
                        ui->lastStepBtn->hide ();
                        ui->nextStepBtn->hide ();

                    }
                }
            }

        }
    }

}


/**
* @brief       人脸删除处理
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::faceDelReply(QNetworkReply *reply){

    qDebug()<<"----删除photo:----"<<reply->error()<<endl;
    //拿到返回结果
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        //打印结果
        qDebug()<<"----处理反斜杠:----"<< all <<endl;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
            if(doucment.isObject()){  // JSON 文档为对象
                QJsonObject object = doucment.object();  //  转化为对象

                qDebug()<< "----faceDelete result:----"<<QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","") <<endl;
                /*
                    删除成功
                    {
                        "log_id": 2764578038040222
                    }
                    删除失败

                    {
                        "error_code": 216616,
                        "log_id": 674786177,
                        "error_msg": "image exist"
                    }
                 */
                QString error_msg="";

                //删除失败
                if(object.contains("error_code")){
                    QJsonValue numValue = object.value("error_code");
                    QJsonValue numValue2 = object.value("error_msg");
                    if(numValue.isDouble()){
                        //                        error_code = numValue.toInt();
                    }

                    qDebug() <<"delete:"<< "人脸删除失败"<<endl;
                    qDebug()<< "----delete failure:----"<< error_msg <<endl;
                    //                    QMessageBox::information(this, QString::fromUtf8("警告!"),QString::fromUtf8("人脸注册失败!"));
                }
                //删除成功
                else{
                    //取log_id  16位字符串
                    log_id =  QString(doucment.toJson()).replace("\n","").replace("\"","").replace(" ","").mid(8,16);
                    //                    qDebug() <<"log_id:"<<log_id<<endl;
                    qDebug() <<"delete:"<< "人脸删除成功"<<endl;
                }
            }
        }
    }
}



/**
* @brief       身份证识别返回结果处理
* @author        胡帅成
* @date        2018-09-07
*/
void MainWindow::dealIdCardReply(Information information,int exitCode)
{


    qDebug()<<"exitCode:"<< exitCode << endl;


    if(exitCode == 0)
    {
        idCardInformation = information;
        qDebug()<<"idCardInformation:"<< idCardInformation.toString ()<< endl;
        allInterface::getinstance ()->info.setidCardInformation (QString::fromStdString(information.getIdNumber ()));
        allInterface::getinstance ()->getIdCardThread ();
        connect (allInterface::getinstance (),SIGNAL(readUserDone()),this,SLOT(dealGetIdCard()));
    }
//        switch (idCheckId) {
//        case 0://未登录
//            break;
//        case 1:// 注册身份证
//            ui->idName_2->setText(QString::fromStdString(information.getName()));
//            ui->idBirth_2->setText(QString::fromStdString(information.getBirthday()));
//            ui->idCardNum_2->setText(QString::fromStdString(information.getIdNumber()));
//            ui->idHomeAddre_2->setText(QString::fromStdString(information.getAddress()));
//            ui->idSex_2->setText(QString::fromStdString(information.getSex()));
//            ui->idNation_2->setText(QString::fromStdString(information.getNation()));
//            ui->idProvePlace_2->setText(QString::fromStdString(information.getSigningOrganization()));
//            ui->idStartDay_2->setText(QString::fromStdString(information.getValidityPeriodStart()));
//            ui->idEndDay_2->setText(QString::fromStdString(information.getValidityPeriodEnd()));
//            ui->idNewestAddre_2->setText(QString::fromStdString(information.getNewestAddress()));

//            player->stop();
//            this->sendPlayText("身份证识别成功！");
//            QMessageBox::information(this, QString::fromUtf8("成功"),QString::fromUtf8("识别成功"));
//            break;
//        case 2://身份证登录+
//            if(!idCardInformation.getName().empty())
//            {
//                qDebug()<<"idCardInformation:不为空:"<< QString::fromStdString(idCardInformation.getIdNumber())<<endl;
//                //判断information不为空
//                if(database.CheckUserByIdCard(QString::fromStdString(idCardInformation.getIdNumber())))
//                {
//                    player->stop();
//                    this->sendPlayText("身份证登录成功");

//                    loginStatus = 2;//身份证登录状态
//                    isIdcardChecked = true;
//                    //用户表中有此用户,可以登录
//                    loginUser  = database.SearchUserByIdNumber(QString::fromStdString(idCardInformation.getIdNumber()));
//                    loginInformation = database.searchIdCardByIdNumber(QString::fromStdString(idCardInformation.getIdNumber()));//以本地数据库注册信息为准
//                    //展示登录成功界面
//                    ui->idUsername->setText("欢迎用户："+loginUser.getUsername());

//                    ui->idLabel->show();
//                    ui->idLabel2->show();
//                    ui->idLabel3->show();
//                    ui->idLabel4->show();
//                    ui->idLabel5->show();

//                    ui->idName->setText(QString::fromStdString(information.getName()));
//                    ui->idAddress->setText(QString::fromStdString(information.getAddress()));
//                    ui->idCardNumber->setText(QString::fromStdString(information.getIdNumber()));
//                    ui->idSex->setText(QString::fromStdString(information.getSex()));
//                    ui->idPhone->setText(loginUser.getPhoneNumber());
//                    //隐藏数据显示框
//                    ui->idName->show();
//                    ui->idAddress->show();
//                    ui->idCardNumber->show();
//                    ui->idPhone->show();
//                    ui->idSex->show();
//                }
//                else
//                {
//                    player->stop();
//                    this->sendPlayText("此身份信息未认证！");

//                    loginStatus = 0;
//                    QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("身份信息未注册！"));
//                    currentIndex =2;
//                    ui->mainWidget->setCurrentIndex(currentIndex);
//                }
//            }
//            break;
//        default:
//            break;
//        }


//    }
//    else
//    {

//        player->stop();
//        this->sendPlayText("身份证信息读取有误！");
//        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("身份证读取有误！"));
//        //跳转到登录选择页
//        //身份信息读取有误后回到原页面
//        if(idCheckId==1)
//        {
//            toCurrentPage (7);}
//        else
//        {
////            on_idBtn_clicked();
//            currentIndex =2;
//            ui->mainWidget->setCurrentIndex (currentIndex);
//        }
//        //        ui->mainWidget->setCurrentIndex(2);
//    }
}
void MainWindow::dealGetIdCard ()
{
    qDebug()<<"登录成功信息"<<allInterface::getinstance ()->info.getmsg ();
    if(allInterface::getinstance ()->info.getmsg ()=="登录成功")
    {
        loginStatus = 2;//身份证登录状态
        isIdcardChecked = true;
        currentIndex = 9;
        ui->mainWidget->setCurrentIndex (currentIndex);
        ui->lastStepBtn->hide ();
        ui->nextStepBtn->hide ();
    }
}

/**
* @function   sendPhoto
* @brief         发送图片查找人脸库
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::sendPhoto(int id,QImage image){

    if(idFace == 2 && !isFaceOk)
    {
        QByteArray fdata = this->getPixmapData("/files/face",image);
        manager  = new QNetworkAccessManager(this);
        //发送结束后，进行人脸结果处理
        QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceReply(QNetworkReply*)));

        //封装请求参数(看接口文档)
        QUrlQuery params;
        fdata = fdata.toBase64().replace("+","-").replace("/","_");
        params.addQueryItem("image",fdata);
        params.addQueryItem("group_id","1");
        params.addQueryItem("imageType",".jpg");

        QString  data = params.toString();
        QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/search/"));
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
        manager->post(request,params.toString().toUtf8());
        qDebug() <<"11111111111111111";
    }
}

/**
* @function   sendPhoto
* @brief         删除人脸
* @author        胡想成
* @date          2018-08-16
*/
void MainWindow::deletePhoto(QString uid){
    qDebug()<<"deletePhoto:"<< uid;
    //准备发送网络请求和接收应答
    manager = new QNetworkAccessManager(this);
    //发送结束后，进行人脸结果处理
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(faceDelReply(QNetworkReply*)));

    //封装请求参数(看接口文档)
    QUrlQuery params;
    params.addQueryItem("uid", uid);
    params.addQueryItem("group_id","1");
    QString  data = params.toString();
    //    QString address = HOST.append("/face/delete/");

    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/face/search/"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->post(request,params.toString().toUtf8());
}



/**
* @brief         获取编码图片
* @author        胡帅成
* @date        2018-09-13
*/
QByteArray MainWindow::getPixmapData(QString filePath,QImage image)
{
    //    qDebug()<<"发送照片";
    QString baseDir = QCoreApplication::applicationDirPath();
    QString fileDir = baseDir.append(filePath);

    //创建目录
    QDir dir;
    if(!dir.exists(fileDir))
    {
        bool res = dir.mkpath(fileDir);
        qDebug() << "创建人脸图片目录结果:" << res;
    }

    fileDir.append(commonutils.getCurrentTime());
    fileDir.append(".jpg");

    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap.save(fileDir);
    qDebug()<<fileDir<<endl;

    QFile file(fileDir);

    if (!file.open(QIODevice::ReadOnly)||file.size()==0)
    {
        file.close();
        qDebug() << "文件打开失败";
    }
    QByteArray fdata = file.readAll();
    file.close();
    return fdata;
}


/**
* @brief       票据识别返回处理
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::billReply(QNetworkReply * reply){
    //打印结果
    qDebug() << "------票据识别结果------"<<reply->error()<< endl;
    if(reply->error() ==    QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        qDebug()<<endl<<endl<<all<<endl;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        if((!doucment.isEmpty()) && jsonError.error == QJsonParseError::NoError){//解析未发生错误
            if(doucment.isObject()){
                QJsonObject object = doucment.object();
                if(object.contains("success")){
                    int success = 0;
                    QJsonValue successVal = object.value("success");
                    if(successVal.isDouble()){
                        success = successVal.toInt();
                    }
                    if(success == 1){
                        //保存票据电子版附件
                        QString baseDir = QCoreApplication::applicationDirPath();
                        QString fileDir = baseDir.append("/files/bill/");
                        //创建目录
                        QDir dir;
                        if(!dir.exists(fileDir))
                        {
                            bool res = dir.mkpath(fileDir);
                            qDebug() << "创建保存票据电子版图片目录结果:" << res;
                        }
                        fileDir.append(commonutils.getCurrentTime());

                        //获得票据附件名称
                        QFileInfo filinfo;
                        filinfo = QFileInfo(fileDir);
                        billname = filinfo.fileName();
                        qDebug()<<"billname"<<billname;
                        fileDir.append(".jpg");
                        billdir = fileDir;
                        qDebug()<<"billdir"<<billdir;
                        blpath.append(billdir);
                        QImage img(billpath);
                        QPixmap pixmap = QPixmap::fromImage(img);
                        pixmap.save(fileDir);
                        bldir.append(billname);//保存票据识别的附件名称
                        allInterface::getinstance ()->info.setuploadPath (billdir);
                        allInterface::getinstance ()->getUploadFile ();
                        int billType = 0;
                        if(object.contains("type")){
                            QJsonValue typeVal = object.value("type");
                            if(typeVal.isDouble()){
                                billType = typeVal.toInt();
                                bill.setBilltype(QString(billType));//票据类别
                            }
                        }
                        QJsonObject infoObj;
                        if(object.contains("info")){
                            QJsonValue infoVal = object.value("info");
                            if(infoVal.isObject()){
                                infoObj = infoVal.toObject();
                            }
                        }

                        /* {
                            "success": 1,
                            "type": 1,
                            "info": {
                                "code": "1100171320",
                                "number": "39283562",
                                "date": "2017年08月13日",
                                "goods": "餐费",
                                "price": "1888.00",
                                "checkCode": "04844754757667122571"
                            }
                        }
                        */

                        /*
                            QString billID;//报销人id
                            QString billExpenseid;
                            QString billAttachmenttitle;//附件名称
                            QString billRealpath;//文件保存路径
                            QString billType;//票据类型
                            QString billContent;//票据名目
                            QString billCode;//票据代码
                            QString billNumber;//票据号码
                            QString billUser;//火车票、飞机票乘车人
                            QString billStartplace;//始发站
                            QString billEndplace;//终点站
                            QString billDate;//发票日期、乘车日期
                            QString billMoney;//票据金额
                            QString billCheckcode;//校验码
                            QString billOther;//发票的号码*/

                        QString type;
                        QString billContent;
                        QString user;
                        QString start;
                        QString end;
                        QString billDate;
                        QString money;
                        QString other;
                        switch (billType) {
                        case 1://发票
                            type = "发票";
                            costType =true;
                            player->stop();
                            this->sendPlayText("发票识别成功");
                            bill.setBilltype(type);
                            if(infoObj.contains("goods")){
                                QJsonValue goodsVal = infoObj.value("goods");
                                if(goodsVal.isString()){
                                    billContent = goodsVal.toString();
                                    billGoods = billContent;
                                    qDebug()<<"billGoods:"<<billGoods;
                                    bill.setBillcontent(billContent);
                                }
                            }
                            if(infoObj.contains("date")){
                                QJsonValue dateVal = infoObj.value("date");
                                if(dateVal.isString()){
                                    fbillDate = dateVal.toString();

                                    bill.setBilldate(fbillDate);
                                }
                            }
                            if(infoObj.contains("price")){
                                QJsonValue priceVal = infoObj.value("price");
                                if(priceVal.isString()){
                                    money = priceVal.toString();
                                    billPrice = money;
                                    bill.setBillmoney(money);
                                }
                            }
                            if(infoObj.contains("number")){
                                QJsonValue numberVal = infoObj.value("number");
                                if(numberVal.isString()){
                                    other = numberVal.toString();
                                    billNumber = other;
                                    bill.setBillnumber(other);
                                }
                            }
                            if(infoObj.contains("code")){
                                QJsonValue numberVal = infoObj.value("code");
                                if(numberVal.isString()){
                                    billCode = numberVal.toString();
                                    bill.setBillcode(numberVal.toString());
                                }
                            }
                            if(infoObj.contains("checkCode")){
                                QJsonValue numberVal = infoObj.value("checkCode");
                                if(numberVal.isString()){
                                    billCheckcode = numberVal.toString();
                                    bill.setBillcheckcode(numberVal.toString());
                                }

                            }
//                            emit costDone ();

                            break;
                        case 2://火车票
                            busiType =true;
                            player->stop();
                            this->sendPlayText("火车票识别成功");
                            bill.setBilltype("火车票");
                            type = "火车票";
                            bill.setBilltype(type);

                            if(infoObj.contains("person")){
                                QJsonValue personVal = infoObj.value("person");
                                if(personVal.isString()){
                                    user = personVal.toString();
                                    trainPerson =user;
                                    bill.setBilluser(user);
                                }
                            }
                            if(infoObj.contains("date")){
                                QJsonValue dateVal = infoObj.value("date");
                                if(dateVal.isString()){
                                    billDate = dateVal.toString();
                                    trainDate = billDate;
                                    bill.setBilldate(billDate);
                                }
                            }
                            if(infoObj.contains("departure")){
                                QJsonValue departureVal = infoObj.value("departure");
                                if(departureVal.isString()){
                                    start = departureVal.toString();
                                    trainStart = start;
                                    bill.setBillstartplace(start);
                                }
                            }
                            if(infoObj.contains("price")){
                                QJsonValue priceVal = infoObj.value("price");
                                if(priceVal.isString()){
                                    money = priceVal.toString();
                                    trainPrice = money;
                                    bill.setBillmoney(money);
                                }
                            }
                            if(infoObj.contains("destination")){
                                QJsonValue destinationVal = infoObj.value("destination");
                                if(destinationVal.isString()){
                                    end = destinationVal.toString();
                                    trainEnd = end;
                                    bill.setBillendplace(end);
                                }
                            }
//                            emit busiDone ();
                            break;
                        case 3://飞机票
                            abroadType = true;
                            player->stop();
                            this->sendPlayText("飞机票识别成功");
                            type = "飞机票";
                            bill.setBilltype(type);

                            if(infoObj.contains("person")){
                                QJsonValue personVal = infoObj.value("person");
                                if(personVal.isString()){
                                    user = personVal.toString();
                                    bill.setBilluser(user);
                                }
                            }
                            if(infoObj.contains("date")){
                                QJsonValue dateVal = infoObj.value("date");
                                if(dateVal.isString()){
                                    billDate = dateVal.toString();
                                    bill.setBilldate(billDate);
                                }
                            }
                            if(infoObj.contains("departure")){
                                QJsonValue departureVal = infoObj.value("departure");
                                if(departureVal.isString()){
                                    start = departureVal.toString();
                                    bill.setBillstartplace(start);
                                }
                            }
                            if(infoObj.contains("price")){
                                QJsonValue priceVal = infoObj.value("price");
                                if(priceVal.isString()){
                                    money = priceVal.toString();
                                    bill.setBillmoney(money);
                                }
                            }
                            if(infoObj.contains("destination")){
                                QJsonValue destinationVal = infoObj.value("destination");
                                if(destinationVal.isString()){
                                    end = destinationVal.toString();
                                    bill.setBillendplace(end);
                                }
                            }
//                            emit abroadDone ();

                            break;
                        case 4://出租车票
                            player->stop();
                            this->sendPlayText("出租车票识别成功");
                            type = "出租车票";
                            bill.setBilltype(type);

                        default:
                            break;
                        }

                        QListWidgetItem *item = new QListWidgetItem();
                        item->setSizeHint(QSize(760,90));

                        billItem *billItemView = new billItem();
                        billItemView->setBillInfo(bill);

                        billItemView->setBillType(billType);
                        billItemView->setBillAccount(bill.getBillmoney());
                        billItemView->setBillPixmap(pixmap);
                        if(billType == 2 || billType == 3) {
                            billItemView->setDeparture(bill.getBillstartplace());
                            billItemView->setDestination(bill.getBillendplace());
                        }

                        ui->billListWidget->addItem(item);
                        ui->billListWidget->setItemWidget(item, billItemView);

                        billList.append(bill);

                        int row = ui->billListWidget->row(item);
                        billItemView->setIndex(row);

                        connect(billItemView, &billItem::openBillItem, this, &MainWindow::openBillItemDialog);
                        connect(billItemView, &billItem::deleteBillItem, this, &MainWindow::deleteBillItemView);

                    }
                }
            }
        }

    }

}


/**
* @brief      语音识别返回处理
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::voiceReply(QNetworkReply * reply){
    //    qDebug()<<"语音识别返回结果处理:"<< reply->error() << endl;

    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        //        qDebug()<<"开始识别语音："<<all<<endl;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        qDebug()<<"识别语音："<<QString(doucment.toJson()).replace("\n","").replace("\"","")<<endl;
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {//JSON转换未出错
            if(doucment.isObject()){ // JSON 文档为对象
                QJsonObject object = doucment.object();
                int statusCode = 0;//定义statusCode
                if(object.contains("statusCode")){
                    QJsonValue statusCodeVal = object.value("statusCode");
                    statusCode = statusCodeVal.toString().toInt();
                    qDebug() << "statusCode:" << statusCode << endl;
                }

                //如果statusCode == 1,识别成功，获取指令编码和识别字符串
                if(statusCode == 1){
                    QString instruction_number;
                    QString resultStr;
                    //获取指令编码
                    if(object.contains("instruction_number")){
                        QJsonValue instructionVal = object.value("instruction_number");
                        if(instructionVal.isString()){
                            instruction_number = instructionVal.toString().trimmed();
                            qDebug() << "1111111"<<instruction_number;
                        }
                    }
                    qDebug()<<"instruction_number:"<<instruction_number<<endl;
                    //获取识别字符串并拼接
                    if(object.contains("content")){
                        QJsonValue contentVal = object.value("content");
                        if(contentVal.isObject()){
                            QJsonObject contentObj = contentVal.toObject();
                            if(contentObj.contains("result")){
                                QJsonValue resultsVal = contentObj.value("result");
                                if(resultsVal.isArray()){
                                    QJsonArray resultArray =resultsVal.toArray();
                                    int size = resultArray.size();
                                    for(int i = 0; i < size ; i++){
                                        QJsonValue resultVal = resultArray.at(i);
                                        if(resultVal.isString()){
                                            //                                            QString resultStrT = resultVal.toString().trimmed();
                                            //去掉逗号
                                            resultStr = resultVal.toString().trimmed();

                                            qDebug() << "resultStrT:" << resultStr << endl;
                                            //                                            resultStrT.replace(resultStrT.length()-1,1,"");
                                            //                                            resultStr.append(resultStrT);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    qDebug()<<"返回结果："<<resultStr<<endl;
                    this->instructionExp(instruction_number);
                }

                else
                {
                    qDebug()<<"语音识别失败:"<<endl;

                }
            }
        }
    }
}

/**
* @brief       语音合成结果处理
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::playReply(QNetworkReply * reply){
    if(reply->error() ==    QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        //        qDebug()<<"语音合成输出：";
        //        qDebug()<<"语音合成输出："<<all<<endl;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(all, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
            if(doucment.isObject()){
                QJsonObject object = doucment.object();
                int statusCode = 0;
                if(object.contains("statusCode")){
                    QJsonValue statusVal = object.value("statusCode");
                    if(statusVal.isString()){
                        statusCode = statusVal.toString().toInt();
                    }
                }
                qDebug()<<statusCode<<endl;
                if(statusCode == 1){
                    if(object.contains("content")){
                        QJsonValue contentVal = object.value("content");
                        QByteArray playData;
                        if(contentVal.isString()){
                            playData = contentVal.toString().toLocal8Bit();
                            playData = playData.replace("-","+");
                            playData = playData.replace("_","/");
                            playData = QByteArray::fromBase64(playData);

                            QString baseDir = QCoreApplication::applicationDirPath();
                            QString fileDir = baseDir.append("/files/player/");

                            //文件夹不存在的时候创建
                            QDir dir;
                            if(!dir.exists(fileDir))
                            {
                                bool res = dir.mkpath(fileDir);
                                qDebug() << "创建目录结果:" << res;
                            }

                            fileDir.append(commonutils.getCurrentTime());
                            fileDir.append(".wav");
                            QFile file(fileDir);
                            file.open(QIODevice::WriteOnly);
                            QDataStream out(&file);
                            out<<playData;
                            file.close();
                            //播放前，需停止语音识别功能
                            audioRecorder->stop();
                            player->setMedia(QUrl::fromLocalFile(fileDir));
                            player->play();
                            playStatus = true;
                        }
                    }
                }
            }
        }
    }
}


//-------------------------------------------slots end----------------------------------------



//-------------------------------------------function start----------------------------------------

/**
* @brief         设置录音机
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::setRecord(){
    audioRecorder->setAudioInput(audioRecorder->audioInputs().at(0));
    audioRecorder->setContainerFormat("wav");
    QAudioEncoderSettings settings;// 音频编码设置
    settings.setCodec("audio/PCM");  //编码
    settings.setSampleRate(16000); //采样率
    settings.setBitRate(128000); //比特率
    settings.setChannelCount(1); //通道数
    settings.setQuality(QMultimedia::VeryHighQuality); //品质
    settings.setEncodingMode(QMultimedia::ConstantBitRateEncoding); //编码模式
    QString container = "wav";
    //    QString container = "audio/x-wav";

    //待解决
    audioRecorder->setAudioSettings (settings);
    //audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);
}

/**
* @brief         开始录音
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::startRecord(){
    QString baseDir = QCoreApplication::applicationDirPath();
    QString fileDir = baseDir.append("/files/records/");

    //创建目录
    QDir dir;
    if(!dir.exists(fileDir))
    {
        bool res = dir.mkpath(fileDir);
        qDebug() << "录音保存目录:" << res;
    }

    fileDir.append(commonutils.getCurrentTime());
    fileDir.append(".wav");
    audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileDir));
    audioRecorder->record();
}

/**
* @brief         结束录音
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::stopRecord(){
    audioRecorder->stop();
    QString fileDir = audioRecorder->outputLocation().toLocalFile();
    qDebug()<<endl<<audioRecorder->outputLocation().toLocalFile()<<endl;
    QFile file(fileDir);
    //发送语音指令，进行识别
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(voiceReply(QNetworkReply*)));

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

    QUrlQuery params;

    params.addQueryItem("audio",fdata);
    params.addQueryItem("audioType",".wav");
    QString  data = params.toString();

    this->printFile("voice.txt",fdata);
    //    QString address = HOST.append("/understand/recognition/");

    QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/understand/recognition/"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    manager->post(request,params.toString().toUtf8());

    recordNum = 0;
    recordHigh = 0;
    recordLow = 0;
    recordSend = false;
    this->startRecord();
}



/**
* @brief        语音识别指令处理
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::instructionExp(QString instruction){
    QString instru = instruction.left(4);
    QString returnWords = instruction.mid(7);

    qDebug()<<"instru:"<<instru<<endl;
    qDebug()<<"returnWords:"<<returnWords<<endl;

    //    0005###请填写出国报销单
    //    0006###报销已确认
    //    0007###报销已取消
    //    0008###报销已提交
    //    0010###请重新选择报销单种类
    //    0011###返回上一步
    //    0012###执行下一步
    //    1000###指令不识别，只输入正确指令
    //    0020###请选择登录方式
    //    0021###请讲身份证放在指定位置
    //    0022###请将脸部对准摄像头
    //    0023###请输入账号
    //    0024###登录已取消
    //    0025###正在登录
    //    0026###登录已结束
    //    2000###指令不识别，只输入正确指令
    //    0030###开始注册流程
    //        0031###请输入账号密码
    //        0032###请将脸部对准摄像头
    //        0033###请讲身份证放在指定位置
    //        0034###已跳过身份认证
    //        0035###已注册账号
    //        0036###注册已结束
    //        3000###指令不识别，请输入正确指令
    //        9999###我不知道你在说什么!

    if(VerifyNumber(instru))
    {
        //清空系统状态  待定
        this->on_firstBtn_clicked();

        player->stop();
        this->sendPlayText(returnWords.toUtf8());
        if("0001"==instru){//我?要?报销     //    0001###请选择验证方式
            ui->mainWidget->setCurrentIndex (2);
            //            ui->nextStepBtn->clicked();
        }else if ("0003"==instru){//人脸(识别)?(验证)   //    0003###请将脸部对准摄像头

            this->on_faceBtn_clicked();

        }else if("0002"==instru){//身份证(识别)?(验证)     //    0002###请将身份证放在指定位置
            //            this->on_faceBtn_clicked();
            this->on_idBtn_clicked();

        }/*else if(""==instru && 1==currentIndex){//缺少帐号登录指令
            ui->accountBtn->clicked();
        }*/
//        else if("0004"==instru){//差旅(报销单)     //    0004###请填写差旅报销单
//            //            ui->busiBtn->clicked();
//            this->on_busiBtn_clicked();
//        }else if ("0005"==instru){//出国(报销单)
//            this->on_abroadBtn_clicked();
//        }else if(""==instru && 9==currentIndex){//缺少费用报销单指令
//            ui->busiBtn->clicked();
//        }
    else if("0011"==instru ){//[上一步|返回]
            this->toLastStep();
        }else if("0012"==instru){//[下一步|返回]
            this->toNextStep();
        }/*else if (("0006"==instru ||"0008"==instru)&& 15==currentIndex){//确认/提交(报销)
            ui->confirmBtn->clicked();
        }*/else if ("0009"==instru && 16==currentIndex){//[退出|结束|结束报销]
            ui->logoutBtn->clicked();
        }else if ("0010"==instru && 16==currentIndex){//继续(报销)
            ui->continueExpenseBtn->clicked();
        }else if("0020"==instru && 0==currentIndex){//我?要?登录
            ui->LogBtn->clicked();
            this->sendPlayText(returnWords.toUtf8());//请选择登录方式
        }else if("0030"==instru && 0==currentIndex){//我?要?注册
            ui->RegBtn_2->clicked();
            this->sendPlayText(returnWords.toUtf8());//开始注册流程
        }else if("0021"==instru && 1==currentIndex){//身份证(登录)
            ui->idBtn->clicked();
            this->sendPlayText(returnWords.toUtf8());//请讲身份证放在指定位置
        }else if("0022"==instru && 1==currentIndex){//人脸(登录)
            ui->faceBtn->clicked();
            this->sendPlayText(returnWords.toUtf8());//请将脸部对准摄像头
        }else if("0023"==instru && 1==currentIndex){//账号(登录)
            ui->accountBtn->clicked();
            this->sendPlayText(returnWords.toUtf8());//请输入账号
        }else if("0031"==instru && 5==currentIndex){//账号(注册)

        }
    }else
    {
        player->stop();
        this->sendPlayText(instruction.toUtf8());
    }
}
/**
* @brief      音频播放状态改变时进行
* @author        胡帅成
* @date        2018-09-06
*/
void MainWindow::playChanged()
{
    //    qDebug()<<"player状态："<<player->state()<<endl;
    if( QMediaPlayer::StoppedState==  player->state() && playStatus == true){
        //播放结束时，继续开启语音识别功能
        playStatus = false;
        qDebug()<<"语音录制开始"<<endl;
        recordNum = 0;
        recordHigh = 0;
        recordLow = 0;
        recordSend = false;
        this->startRecord();
    }
}

/**
* @brief         音量级别显示
* @author        胡帅成
* @date        2018-09-06
*/
template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}

// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;

    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    if (buffer.format().codec() != "audio/pcm")
        return values;

    int channelCount = buffer.format().channelCount();
    values.fill(0, channelCount);
    qreal peak_value = getPeakValue(buffer.format());
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

void MainWindow::processBuffer(const QAudioBuffer& buffer)
{
    if (audioLevels.count() != buffer.format().channelCount()) {
        qDeleteAll(audioLevels);
        audioLevels.clear();
        for (int i = 0; i < buffer.format().channelCount(); ++i) {
            QAudioLevel *level = new QAudioLevel();
            audioLevels.append(level);
            ui->levelLayout->addWidget(level);
        }
    }

    QVector<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i)
        audioLevels.at(i)->setLevel(levels.at(i));

    //    qDebug()<<"音量级别："<<levels<<endl;

    recordNum ++;
    if(levels.at(0) >= 0.35){
        recordHigh ++;
        recordLow = 0 ;
    }else{
        recordLow ++;
    }

    //    qDebug()<<"recordNum:"<<recordNum<<endl;
    if(recordNum <= 150){
        //当录音时间小于6秒时
        if(recordHigh >= 2){
            if(recordLow >= 12){
                //当1秒之内无声音时，结束录音
                recordSend = true;
                this->stopRecord();
            }
        }
    }else{
        //当录音时间大于6秒时，结束录音
        if(recordHigh >= 2){
            recordSend = true;
        }else{
            recordSend = false;
        }
        this->stopRecord();
    }
}

/**
* @brief         用户中心入口
* @author        胡帅成
* @date        2018-09-10
*/
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->userInfo)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            //            if(!loginUser.getUsername().isEmpty())
            if(loginStatus!=0)
            {
                player->stop();
                this->sendPlayText("欢迎用户"+loginUser.getUsername().toUtf8());
                userInfoDialog = new UserInfoDialog(this); //单击弹框

                //查询数据库
                if(!loginUser.getIdCard().isEmpty())
                {
                    //身份已认证
                    idCheckId = 2;
                    loginInformation = database.searchIdCardByIdNumber(loginUser.getIdCard());
                }else
                {
                    //未认证
                    idCheckId = 1;
                }
                //能弹框必然已登录
                userInfoDialog->showUserInfo(loginUser,loginInformation,loginStatus ,idCheckId);
                userInfoDialog->show();
                //监控认证页面
                connect(userInfoDialog, SIGNAL(emitIdCheck(User,int)), this, SLOT(dealGetIdCheck(User,int)));
            }else
            {
                //未登录                currentIndex = 1;
                idCheckId = 0;
            }
        }
    }
    //        return MainWindow::eventFilter(watched, event);
    //    return;
}


/**
* @brief         打印日志输出到文件
* @author        胡想成
* @date          2018-08-15
*/
void MainWindow::printFile(QString dataFileName,QByteArray fdata)
{
    //打印请求参数(测试)
    //    QString dataFileName = "reply.txt";
    QFile datafile(dataFileName);
    if(!datafile.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        QMessageBox::warning(this,"file write","can't open",QMessageBox::Yes);
    }
    QTextStream in(&datafile);
    in<<fdata<<"\n";
    datafile.close();
    qDebug()<<"--------get "+dataFileName+"-------"<<endl;
}

/**
* @brief         初始化欢迎页面
* @author        胡帅成
* @date        2018-09-11
*/
void MainWindow::paintEvent(QPaintEvent*)
{
    QString bgFilename = ":/new/prefix1/back4.jpeg";
    QPainter painter(this);
    //绘制背景
    QRect rec;
    rec.setTopLeft(QPoint(0,0));
    rec.setBottomRight(QPoint(this->width(),this->height()));
    QPixmap pix(bgFilename);//优先构造函数
    //    pix.load("../image/cheese.jpeg");
    painter.drawPixmap(rec,pix);
}

//-------------------------------------------function end----------------------------------------





/**
* @brief         跳过身份认证
* @author        胡帅成
* @date        2018-09-11
*/
void MainWindow::on_idCheckIgnoreBtn_clicked()
{
    player->stop();
    this->sendPlayText("跳过身份认证");

    this->idCardThreadStop();

    if(loginStatus==0)
    {
        currentIndex = 2;
        ui->mainWidget->setCurrentIndex(currentIndex);
    }else
    {
        //已登录状态,跳过则回到主页面
        currentIndex =9;
        ui->mainWidget->setCurrentIndex(currentIndex);
        showUserInfo (loginUser);

    }
}

/**
* @brief         身份证读取
* ni
* @author        胡帅成
* @date        2018-09-11
*/
void MainWindow::idCardRead()
{
    //局部线程的创建
    if(m_currentRunLoaclThread)
    {
        m_currentRunLoaclThread->stopImmediately();
    }
    IdCardThread *idCardThread = new IdCardThread(NULL);
    connect(idCardThread, SIGNAL(emitIdInfo(Information, int)), this, SLOT(dealIdCardReply(Information, int)),Qt::QueuedConnection);
    //识别成功则进行下一步，识别失败则返回
    connect(idCardThread,&IdCardThread::finished ,idCardThread,&QObject::deleteLater);//线程结束后调用deleteLater来销毁分配的内存
    connect(idCardThread,&QObject::destroyed,this,&MainWindow::onLocalThreadDestroy);

    idCardThread->start();
    m_currentRunLoaclThread = idCardThread;

}

void MainWindow::onLocalThreadDestroy(QObject *obj)
{
    if(qobject_cast<QObject*>(m_currentRunLoaclThread) == obj)
    {
        m_currentRunLoaclThread = NULL;
    }
}




void MainWindow::showUserInfo(User user)
{
    //    if(!user.getUsername().isEmpty())
    if(loginStatus!=0)
    {
        qDebug()<<"用户已登陆"<<endl;
        ui->userInfo->show();
        ui->userInfo->setText("欢迎用户："+user.getUsername());
        ui->faceManBtn->show();//人脸库管理登陆后才现实
    }else
    {
        ui->userInfo->hide();
        ui->faceManBtn->hide();
    }
}
/**
* @brief         终止后台身份证读取线程
* @author        胡帅成
* @date        2018-09-13
*/
void MainWindow::idCardThreadStop()
{
    qDebug() << "------销毁身份证识别线程："<< endl;

    //局部线程的创建
    if(m_currentRunLoaclThread)
    {
        m_currentRunLoaclThread->stopImmediately();
    }
}

/**
* @brief         注册页图片显示
* @author        胡帅成
* @date        2018-09-17
*/
//void MainWindow::showRegPhoto(QString filePath)
//{
//    qDebug() << "路径:"<< filePath;
//    if(!filePath.isEmpty())
//    {
//        QPixmap avater;
//        avater.load(filePath);

//        QLabel* avaterRegLabel = ui->mainWidget->findChild<QLabel*>("avaterRegLabel"); //根据子控件的名称查找子控件
//        avaterRegLabel->setPixmap(avater);
//        //        ui->avaterRegLabel2->setPixmap(avater);
//    }


//}


//void MainWindow::on_pushButton_3_clicked()
//{
//    qDebug() << "选择文件";
//    //定义文件对话框类
//    QFileDialog *fileDialog = new QFileDialog(this);
//    //定义文件对话框标题
//    fileDialog->setWindowTitle(tr("打开图片"));
//    //设置默认文件路径    fileDialog->setDirectory(".");
//    //设置文件过滤器
//    fileDialog->setNameFilter(tr("Images(*.jpg *.jpeg)"));
//    //设置视图模式
//    fileDialog->setViewMode(QFileDialog::Detail);
//    QStringList fileNames;
//    if(fileDialog->exec()){
//        fileNames = fileDialog->selectedFiles();
//    }
//    //    QString fileName;
//    avaterFilePath = fileNames.at(0);
//    qDebug() <<"avater avaterFilePath: "<< avaterFilePath;
//    ui->avaterLineEdit->setText(avaterFilePath);

//    if(!avaterFilePath.isEmpty())
//    {
//        //判断路径的合法性
//        //        QFile file(avaterFilePath);
//        QFile* file= new QFile(avaterFilePath); //file为二进制数据文件名

//        if (!file->open(QIODevice::ReadOnly))
//        {
//            //            file.close();
//            QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("图片打开失败!"));
//            ui->avaterLineEdit->clear();
//            ui->avaterRegBoxLabel->clear();
//            //            qDebug() << "图片打开失败";
//            //            currentIndex = 5;
//            //            ui->mainWidget->setCurrentIndex(currentIndex);
//            //            ui->avaterLineEdit->clear();
//        }else
//        {
//            //插入头像
//            QByteArray data;
//            //            file.open(QIODevice::ReadOnly);
//            data = file->readAll();
//            QVariant imageData(data);
//            regUser.setAvater(imageData);

//            QPixmap avater;
//            avater.loadFromData(data,"jpg");
//            ui->avaterRegBoxLabel->setPixmap(avater);
//            ui->avaterRegBoxLabel->setScaledContents(true);
//        }
//        file->close();
//    }
//}

void MainWindow::dealGetIdCheck(User user, int currentIndex)
{
    qDebug() << "开始认证";
    regUser = user;
    idCheckId = 1;
    toCurrentPage (7);
}

void MainWindow::on_firstBtn_clicked()
{
//    currentIndex=0;
//    ui->mainWidget->setCurrentIndex(currentIndex);
    //清空loginUser
    loginStatus = 0;
    //    loginUser.setUsername("");
    User user;
    loginUser = user;
    qDebug() << "清空loginUser:" <<loginUser.toString () <<endl;
    this->showUserInfo(loginUser);
    loginStatus=0;
    // 清空idinformation
    Information information;
    idCardInformation = information;
    //资源回收释放.

    switch (currentIndex) {
    case 3: //人脸识别页面
        qDebug() << "test11111111";
        player->stop();
        this->sendPlayText("人脸登录停止,返回首页");
        this->deleteFace ();
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    case 5: //身份证登录页面
        player->stop();
        this->sendPlayText("身份证识别停止,返回首页");
        this->idCardThreadStop ();
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    case 4 : //登录页面
        player->stop();
        this->sendPlayText("登录停止停止,返回首页");
        this->deleteAccountInfo (); //清空账号登录信息.
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    case 6 : //用户名和密码注册页面
        player->stop();
        this->sendPlayText("停止注册,返回首页");
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        this->deleteAccountReg ();
        break;
    case 7 : //人脸注册页面
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("正在人脸注册,请勿退出"));
        break;
    case 8 : //身份证识别页面
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("正在身份识别,请勿退出"));
        break;
    default:
        currentIndex=0;
        ui->mainWidget->setCurrentIndex(currentIndex);
        break;
    }

}
/**
* @brief         人脸库管理
* @author        胡帅成
* @date        2018-10-12
*/
void MainWindow::on_faceManBtn_clicked()
{
    //查询人脸库列表
    faceMangeDialog = new FaceMangeDialog(this);
    faceMangeDialog->show();
}

/**
* @brief       扫描票据
* @author      胡坤伦
* @date        2019-03-07
*/
void MainWindow::on_scanBillbtn_clicked()
{
    costRowcount = 1;
    scanInfoDialog = new scanDialog(this);
    scanInfoDialog->show ();
    connect (scanInfoDialog,&scanDialog::scanDone,this,&MainWindow::billScanRelpy);
}
/**
* @brief       给后端发送票据信息
* @author      胡坤伦
* @date        2019-03-07
*/

void MainWindow::billScanRelpy ()
{
        qDebug () <<"111";
        QString runpath= QCoreApplication::applicationDirPath();
        scanBillpath=runpath+"/finalbill.jpg";

        qDebug()<<"runpath:"<<runpath;
        qDebug() << "scanBillpath:" << scanBillpath;

        manager = new QNetworkAccessManager(this);
        //读取完成后识别票据
        QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(billReply(QNetworkReply*)));

        QFile file(scanBillpath);
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

        QUrlQuery params;
        params.addQueryItem("image",fdata);
        params.addQueryItem("imageType",".jpg");
        QString  data = params.toString();

        //打印票据图片编码
        this->printFile("billReply.txt", fdata);
        //    QString address = HOST.append("/bill/");

        QNetworkRequest request = commonutils.getHttpRequest(address.left(25).append("/bill/"));
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
        manager->post(request,params.toString().toUtf8());
//        scanInfoDialog->close ();
}


bool MainWindow::VerifyNumber(QString str) { //这一步做什么,
    std::string temp = str.toStdString();
    for (int i = 0; i < str.length(); i++)
    {
        if (temp[i]<'0' || temp[i]>'9')
        { return false;
        }
    }
    return true;
}

//void MainWindow::on_carmSkipBtn_clicked()
//{
//    idCheckId = 1;//认证的时候调用
//    //    currentIndex=4;
//    //    ui->mainWidget->setCurrentIndex(currentIndex);
//    //    this->idCardRead();
//    toCurrentPage (7);
//}




void MainWindow::on_talkBtn_clicked()
{
    qmlRegisterType<Turing>("io.qt.turing", 1, 0, "Turning");

    QQuickView *view = new QQuickView();
    QWidget *widget = QWidget::createWindowContainer(view, this);
    //    connect(turing,&Turing::received,this,&MainWindow::turingSpeak);//turing speak
    view->setSource(QUrl("qrc:/main.qml"));
}



void MainWindow::on_expenseBtn_clicked()
{
    toCurrentPage (2);
    ui->RegBtn_2->hide ();
    player->stop();
    this->sendPlayText("请选择登录方式");
}

void MainWindow::on_applyButton_clicked()
{
    currentIndex = 10;
    ui->mainWidget->setCurrentIndex (currentIndex);
    ui->lastStepBtn->hide ();
    ui->nextStepBtn->hide ();
}

void MainWindow::on_advisoryButton_clicked()
{

    this->on_talkBtn_clicked ();
}

void MainWindow::on_costBaseButton_clicked()
{
    if (expenseType == 11)
        currentIndex=11;
    else if (expenseType == 12 || expenseType == 14)
        currentIndex=12;
    ui->mainWidget->setCurrentIndex (currentIndex);
}

void MainWindow::on_costScanButton_clicked()
{
    currentIndex = 13;
    ui->mainWidget->setCurrentIndex (currentIndex);
}

void MainWindow::on_costItemButton_clicked()
{
    if (expenseType == 11)
        currentIndex=14;
    else if (expenseType == 12 || expenseType == 14)
        currentIndex=16;
    ui->mainWidget->setCurrentIndex (currentIndex);
}

void MainWindow::on_costPayButton_clicked()
{
    currentIndex = 15;
    ui->mainWidget->setCurrentIndex (currentIndex);
}
/**
* @brief       显示报销流程按钮
* @author      胡坤伦
* @date        2019-04-01
*/
void MainWindow::isShowguiInforn ()
{
    ui->costBaseButton->show ();
    ui->costBaseLabel->show();
    ui->costScanButton->show ();
    ui->costScanLabel->show ();
    ui->costItemButton->show();
    ui->costItemLabel->show();
    ui->costPayButton->show();
    ui->costPayLabel->show ();
}
/**
* @brief       隐藏报销流程按钮
* @author      胡坤伦
* @date        2019-04-01
*/
void MainWindow::noShowguiInforn ()
{
    ui->costBaseButton->hide ();
    ui->costBaseLabel->hide ();
    ui->costScanButton->hide ();
    ui->costScanLabel->hide ();
    ui->costItemButton->hide ();
    ui->costItemLabel->hide ();
    ui->costPayButton->hide ();
    ui->costPayLabel->hide ();
}

void MainWindow::on_costAgainButton_clicked()
{
    this->on_scanBillbtn_clicked();
}

/**
* @brief       添加支付信息按钮,打开对应的Dialog
* @author      黄梦君
* @date        2019-07-02
*/
void MainWindow::on_addPayItemBtn_clicked()
{
    addPayDlg = new addPayDialog(this);
    addPayDlg->show();

    connect(addPayDlg, &addPayDialog::addPayItem, this, &MainWindow::addPayInfoItem);
}

void MainWindow::on_modifyPayItemBtn_clicked()
{
    int row = ui->payInfoList->currentRow();
    if (row <= -1)
        return;

    addPayDlg = new addPayDialog(this);
    addPayDlg->setIndex(row);

    payItemInfo *info = mPayItemList.takeAt(row);
    addPayDlg->setItem(info);
    connect(addPayDlg, &addPayDialog::addPayItem, this, &MainWindow::addPayInfoItem);

    ui->payInfoList->takeItem(row);

    addPayDlg->show();
}

void MainWindow::on_deletePayItemBtn_clicked()
{
    int row = ui->payInfoList->currentRow();
    if (row <= -1)
        return;

    payItemInfo *info = mPayItemList.takeAt(row);
    ui->payInfoList->takeItem(row);

    addPayAccount();
}

/**
* @brief       添加支付信息
* @brief       支付信息窗口保存退出后，在列表里添加支付信息
* @author      黄梦君
* @date        2019-07-02
*/
void MainWindow::addPayInfoItem(int index, payItemInfo *info)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(720, 50));

    payMethodsItem *widget = new payMethodsItem();
    widget->setInfoItem(info);

    //对于修改的item，在原来的位置添加view
    if (index > -1) {
        ui->payInfoList->insertItem(index, item);
    }
    else {
        ui->payInfoList->addItem(item);
    }
    ui->payInfoList->setItemWidget(item, widget);
    mPayItemList.append(info);

    addPayAccount();
}

void MainWindow::costbaseRead () //获取费用基本报销信息
{
    costBinfo.setcostRnumEidt (ui->costRnumEdit->text ());
    costBinfo.setcostConname (ui->costConnameEdit->text ());
    costBinfo.setcostCono (ui->costConoEdit->text ());
    costBinfo.setcostHandp (ui->costHandpEdit->text ());
    costBinfo.setcostRdate (ui->costRdateEdit->text());
    costBinfo.setcostHandpd (ui->costHandpdEdit->text ());
    costBinfo.setcostUse (ui->costUseEdit->text ());
    database.insertCostBaseInfo (costBinfo);
}
void MainWindow::travelbaseRead (){
    travelBinfo.settravelNumber (0);
    travelBinfo.settraveldate ("");
    travelBinfo.settravelPeople ("");
    travelBinfo.settravelPdpart ("");
    travelBinfo.settravelUse ("");
//    travelBinfo.setisSchool ();
//    travelBinfo.setstaffName ();
//    travelBinfo.setstaffNumber ();
//    travelBinfo.setleaveDate ();
}

void MainWindow::addPayAccount()
{
    int totalAccount = 0;

    for (int i=0; i< mPayItemList.size(); i++)
        totalAccount += mPayItemList.at(i)->getAccount();

    ui->payAccount->setText(QString::number(totalAccount));

    totalPayAccount = totalAccount;
}

/**
* @brief
* @brief       添加费用报销明细按钮
* @author      黄梦君
* @date        2019-07-09
*/
void MainWindow::on_addDetailBtn_clicked()
{
//    if (mReimItemList.size() > 0)
//        emit saveReimDetail();

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(645, 210));

    reimDetail *info = new reimDetail;
    reimDetailItem *detailItem = new reimDetailItem();
    detailItem->setReimDetail(info);

    ui->feeDetailList->addItem(item);
    ui->feeDetailList->setItemWidget(item, detailItem);

    mReimItemList.append(info);

//    int index = ui->feeDetailList->count();
//    qDebug() << "this new detail's index is :" << index;

    qDebug() << "报销明细列表此时有 " << mReimItemList.size() << "个项目！";

    connect(detailItem, &reimDetailItem::changeAccount, this, &MainWindow::addReimAccount);
//    connect(this, &MainWindow::saveReimDetail, detailItem, &reimDetailItem::saveDetail);

    addReimAccount();
}

void MainWindow::addReimAccount()
{
    int totalAccount = 0;

    for (int i=0; i< mReimItemList.size(); i++)
        totalAccount += mReimItemList.at(i)->getAccount();

    ui->reimAccount->setText(QString::number(totalAccount));

    totalReimAccount = totalAccount;
}

/**
* @brief
* @brief       复制费用报销明细按钮
* @author      黄梦君
* @date        2019-07-09
*/
void MainWindow::on_copyDetailBtn_clicked()
{
    //尚未实现
}

/**
* @brief
* @brief       删除费用报销明细按钮
* @author      黄梦君
* @date        2019-07-09
*/
void MainWindow::on_deleteDetailBtn_clicked()
{
    //只有一条报销明细的时候，提示至少保留一条，并添加在页面上
    if(ui->feeDetailList->count() == 1) {
        if (QMessageBox::warning(this, "删除提醒", "至少要保留一条报销明细！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
            ui->feeDetailList->clear();
            on_addDetailBtn_clicked();
        }
    }
    else {
        QListWidgetItem *item = ui->feeDetailList->currentItem();
        int row = ui->feeDetailList->currentRow();
        ui->feeDetailList->removeItemWidget(item);
        mReimItemList.takeAt(row);
        delete item;
    }

    qDebug() << "报销明细列表此时有 " << mReimItemList.size() << "个项目！";
    addReimAccount();
}

//打开要编辑的票据信息窗口
void MainWindow::openBillItemDialog(BillCheck info)
{
    scanInfoDialog = new scanDialog(this);

    scanInfoDialog->setBillCheck(info);
    scanInfoDialog->show ();
    //connect (scanInfoDialog,&scanDialog::scanDone,this,&MainWindow::billScanRelpy);
}

//删除票据信息
void MainWindow::deleteBillItemView(int row)
{
    //int row = ui->billListWidget->currentRow();
    QListWidgetItem *item = ui->billListWidget->takeItem(row);
    billList.takeAt(row);
    delete item;
}

//一次报销完成后，将报销流程中的所有信息清空
void MainWindow::clearAllInput()
{
    //costBaseInfo
    ui->costRnumEdit->clear();
    ui->costConnameEdit->clear();
    ui->costConoEdit->clear();
    ui->costHandpEdit->clear();
    ui->costRdateEdit->clear();
    ui->costHandpdEdit->clear();
    ui->costUseEdit->clear();

    //baseInfo
    ui->docuNumber->clear();
    ui->transactor->clear();
    ui->tranDepartment->clear();
    ui->busiReason->clear();

    //bill
    ui->billListWidget->clear();

    //costItem
    ui->feeDetailList->clear();

    //payinfo
    ui->payInfoList->clear();

    //addPersonnelWidget
    ui->personnelList->clear();
}

//添加人员
void MainWindow::on_addPerBtn_clicked()
{
    addPerDlg = new insertPersonnelDialog(this);
    addPerDlg->show();
    addPerDlg->setType(mPersonType);

    if (mPersonType == 1) {
        connect(addPerDlg, &insertPersonnelDialog::addTravelPerson, this, &MainWindow::addTravelPerson);
    }
    else if (mPersonType == 2) {
        connect(addPerDlg, &insertPersonnelDialog::addAbroadPerson, this, &MainWindow::addAbroadPerson);
    }
}

//添加出差人员
void MainWindow::addTravelPerson(int index, traBusPersonInfo *info)
{
    //使List失去焦点，需要重写方法，clearFocus()无效
    //ui->personnelList->clearFocus();

    if (mPersonType != 1)
        return;
    mTravelPersons.append(info);
//    qDebug() << "travel person list info:" << mTravelPersons ;

    QString strName = info->getStaffName();
    int account = info->getTotalFee();
    QString departure = info->getLeavePlace();
    QString destination = info->getArrivePlace();

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(850, 55));

    personItem *widget = new personItem;
    widget->setPersonName(strName);
    widget->setAccount(account);
    widget->setDeparture(departure);
    widget->setDestination(destination);

    if (index > -1) {
        ui->personnelList->insertItem(index, item);
    } else {
        ui->personnelList->addItem(item);
    }

    ui->personnelList->setItemWidget(item, widget);

    addPersonAccount();

}

//添加出国人员
void MainWindow::addAbroadPerson(int index, abroadPersonInfo *info)
{
    //使List失去焦点，需要重写方法，clearFocus()无效
    //ui->personnelList->clearFocus();

    if (mPersonType != 2)
        return;
    mAbroadPersons.append(info);
//    qDebug() << "abroad person list info:" << mAbroadPersons;

    QString strName = info->getStaffName();
    int account = info->getTotalFee();
    QString departure = info->getLeaveCity();
    QString destination = info->getArriveCity();

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(850, 55));

    personItem *widget = new personItem;
    widget->setPersonName(strName);
    widget->setAccount(account);
    widget->setDeparture(departure);
    widget->setDestination(destination);

    if (index > -1) {
        ui->personnelList->insertItem(index, item);
    } else {
        ui->personnelList->addItem(item);
    }
    ui->personnelList->setItemWidget(item, widget);

    addPersonAccount();
}

//删除人员
void MainWindow::on_delPerBtn_clicked()
{
    int row = ui->personnelList->currentRow();
    if (row <= -1)
        return;

    if(mPersonType == 1) {
        traBusPersonInfo *info = mTravelPersons.takeAt(row);
    } else if (mPersonType == 2) {
        abroadPersonInfo *info = mAbroadPersons.takeAt(row);
    }
    ui->personnelList->takeItem(row);
    addPersonAccount();
}

//修改人员
void MainWindow::on_modifyPerBtn_clicked()
{
    int row = ui->personnelList->currentRow();
    if (row <= -1)
        return;

    addPerDlg = new insertPersonnelDialog(this);
    addPerDlg->setType(mPersonType);
    addPerDlg->setIndex(row);
    if (mPersonType == 1) {
        traBusPersonInfo *info = mTravelPersons.takeAt(row);
        addPerDlg->setTravelPerson(info);
        connect(addPerDlg, &insertPersonnelDialog::addTravelPerson, this, &MainWindow::addTravelPerson);
    }
    else if (mPersonType == 2) {
        abroadPersonInfo *info = mAbroadPersons.takeAt(row);
        addPerDlg->setAbroadPerson(info);
        connect(addPerDlg, &insertPersonnelDialog::addAbroadPerson, this, &MainWindow::addAbroadPerson);
    }
    ui->personnelList->takeItem(row);
    addPerDlg->show();
}

//获得人员总金额
void MainWindow::addPersonAccount()
{
    int totalAccount = 0;
    if (mPersonType == 1) {
        for (int i=0; i< mTravelPersons.size(); i++)
            totalAccount += mTravelPersons.at(i)->getTotalFee();
    }
    else if (mPersonType == 2) {
        for (int i=0; i< mAbroadPersons.size(); i++)
            totalAccount += mAbroadPersons.at(i)->getTotalFee();
    }
    ui->totalAccount->setNum(totalAccount);

    totalPerAccount = totalAccount;
}

void MainWindow::on_savePayBtn_clicked()
{
    QDateTime local(QDateTime::currentDateTime());

    if (expenseType == 11) {
        if (totalPayAccount > totalReimAccount) {
            QMessageBox::warning(this, "支付总金额问题", "支付总金额不得超过报销总金额", QMessageBox::Ok);
        } else {
            QJsonObject object;
            QJsonArray attachementList;
            QJsonArray detailList;
            QJsonArray payList;
            QJsonArray updateRecordList;
            QJsonDocument document;

            QJsonObject attachement;
            for (int i=0; i<billList.size(); i++) {
                BillCheck bill = billList.at(i);

                attachement.insert("billid", bill.getBillid());            //单据id string
                attachement.insert("extend", bill.getBillexpenseid());      //拓展名 string
                attachement.insert("id", bill.getBillid());                //ID唯一标示
                attachement.insert("invoiceNum", bill.getBusid());        //发票号 string
                attachement.insert("invoiceType", "1");                     //发票类型(0纸制1电子) string
                attachement.insert("name", bill.getBillattachmenttitle());  //附件名称 string
                attachement.insert("path", bill.getBillrealpath());         //附件路径 string
                attachement.insert("reviewpath", bill.getBillrealpath());   //预览路径 string
                attachement.insert("size", 0);                           //附件大小 int
                attachement.insert("swfpath", "");                          //swf格式路径 string
                attachement.insert("type", "");                             //附件类型 string

                attachementList.append(QJsonValue(attachement));
            }

            QJsonObject detail;
            for (int i=0; i<mReimItemList.size(); i++) {
                reimDetail *info = mReimItemList.at(i);

                detail.insert("billId", "");   //(string)表头单据id
                detail.insert("budgetDepartId", "");             //(string)预算归口部门id
                detail.insert("budgetSubjectId", info->getBudgetNumber());    //预算科目id string
                detail.insert("budgetSubjectName", info->getBudgetName());  //预算科目名称 string
                detail.insert("busiId", info->getItem());  //业务事项(字典: dict_data_business_matters) string
                detail.insert("busiName", info->getItem());          //(string)业务事项名称
                detail.insert("feeDepartId", info->getDepartment());            //(string)费用承担科室id
                detail.insert("fundType", info->getFeeType());     //(string)经费类型
                detail.insert("id", "");       //string
                detail.insert("memo", "");                   //(string)备注
                detail.insert("moneyReim", info->getAccount());           //(number)报销金额
                detail.insert("projectId", "");        //(string)项目id
                detail.insert("projectName", "");           //(string)项目名称

                detailList.append(QJsonValue(detail));
            }

            QJsonObject pay;
            for (int i=0 ; i<mPayItemList.size() ; i++) {
                payItemInfo *info = mPayItemList.at(i);

                pay.insert("bankAddress", info->getBankName());   //(string)开户银行
                pay.insert("bankName", info->getBank());      //(string)所属银行
                pay.insert("bankNo", info->getBankcardNumber());     //(string)银行账号
                pay.insert("billId", "");        //(string)单据id
                pay.insert("busiCardNo", "");    //(string)公务卡卡号
                pay.insert("cardDate", info->getCardDate().toString()); //(string)刷卡日期
                pay.insert("cardNo", info->getBankcardNumber());    //(string)银行卡卡号
                pay.insert("checkNum", "");      //(string)支票号
                pay.insert("collectPeopleId", "");   //(string)收款人/单位id
                pay.insert("collectPeopleName", info->getPayeeName());     //(string)收款人/单位名称
                pay.insert("counterPartName", info->getDepartment());       //(string)对方单位名称
                pay.insert("id", "");            //(string)
                pay.insert("jkCode", "");        //(string)借款单编号
                pay.insert("jkId", "");          //(string)借款单id
                pay.insert("memo", info->getRemark());          //(string)备注
                pay.insert("money", info->getAccount());         //(number)金额
                pay.insert("moneyBack", 0);     //(number)应归还金额
                pay.insert("moneyCard", 0);     //(number)刷卡金额
                pay.insert("moneyLoan", 0);     //(number)借款金额
                pay.insert("moneyOffset", 0);   //(number)冲借款金额
                pay.insert("payeeCity", info->getCity());     //(string)收款人省市
                pay.insert("paymethod", info->getStrType());     //(string)支付方式

                payList.append(QJsonValue(pay));
            }

            QJsonObject updateRecord;
            for (int i=0; i<1; i++) {
                updateRecord.insert("billId", "");    //(string)单据id
                updateRecord.insert("dict", "");    //(string)变更字典
                updateRecord.insert("field", "");    //(string)字段
                updateRecord.insert("id", "");    //(string)ID唯一标识
                updateRecord.insert("name", "");    //(string)明细标识名称
                updateRecord.insert("type", 0);    //(integer)修改类型
                updateRecord.insert("updateString", "");    //(string)变更字符串
                updateRecord.insert("valueUpdateAfter", "");    //(string)修改后数值
                updateRecord.insert("valueUpdateBefore", "");    //(string)修改前数值

                updateRecordList.append(QJsonValue(updateRecord));
            }

            object.insert("attachementList", QJsonValue(attachementList));
            object.insert("audit", 0);	//单据审核状态 int
            object.insert("auditFlag", "");	//是否查询审批标识 string
            object.insert("auditState", 1);	//审批状态 int
            object.insert("billAmount", billList.size());	//单据张数 int
            object.insert("billDate", local.toString("yyyy-MM-dd"));	//填报日期 string
            object.insert("code", "");      //单据编号 string
            object.insert("contractCode", "");	//(string)合同编号
            object.insert("contractId", "");	//(string)合同id
            object.insert("contractName", "");	//(string)合同名称
            object.insert("currency", "");	//(string)币种
            object.insert("departId", chargeDepartment);	//经办人部门 string
            object.insert("detailList", QJsonValue(detailList));
            object.insert("id", "");		//唯一标识 string
            object.insert("isContract", "");    //(string)是否有合同
            object.insert("isElecTicket", "");//是否有电子发票号 string
            object.insert("moneyBack", 0);	//应还款金额合计 number
            object.insert("moneyContract", totalPayAccount); //(number)合同总金额
            object.insert("moneyHandling", 0);	//手续费 number
            object.insert("moneyOffset", 0);	//冲款金额合计
            object.insert("moneyReim", totalReimAccount);	//报销金额合计
            object.insert("moneyReissue", 0);//应补发金额合计
            object.insert("operator", chargeManName);	//经办人 string
            object.insert("payDate", local.toString("yyyy-MM-dd"));	//支付日期 string
            object.insert("payList", QJsonValue(payList));
            object.insert("period", "");	//单据所属区间 string
            object.insert("procInsId", "");	//流程实例Id string
            object.insert("projectType", "");	//项目类型 string
            object.insert("reportPerson", 0);//填报人 int
            object.insert("submitAudit", "");	//(string)是否是提交
            object.insert("ticketNo", "");	//(string)支票号
            object.insert("updateRecordList", QJsonValue(updateRecordList));
            object.insert("use", "");   //(string)用途


            document.setObject(object);
            QByteArray bytes = document.toJson(QJsonDocument::Compact);
            QString strBytes(bytes);

            qDebug() << strBytes;
        }
    }
    else if (expenseType == 12) {
        if (totalPayAccount > totalPerAccount) {
            QMessageBox::warning(this, "支付总金额问题", "支付总金额不得超过人员补贴总金额", QMessageBox::Ok);
        } else {
            QJsonObject object;
            QJsonArray attachementList;
            QJsonArray detailList;
            QJsonArray payList;
            QJsonDocument document1;

            QJsonObject attachement;
            for (int i=0; i<billList.size(); i++) {
                BillCheck bill = billList.at(i);

                attachement.insert("billid", bill.getBillid());            //单据id string
                attachement.insert("extend", bill.getBillexpenseid());      //拓展名 string
                attachement.insert("id", bill.getBillid());                //ID唯一标示
                attachement.insert("invoiceNum", bill.getBusid());        //发票号 string
                attachement.insert("invoiceType", "1");                     //发票类型(0纸制1电子) string
                attachement.insert("name", bill.getBillattachmenttitle());  //附件名称 string
                attachement.insert("path", bill.getBillrealpath());         //附件路径 string
                attachement.insert("reviewpath", bill.getBillrealpath());   //预览路径 string
                attachement.insert("size", 0);                           //附件大小 int
                attachement.insert("swfpath", "");                          //swf格式路径 string
                attachement.insert("type", "");                             //附件类型 string

                attachementList.append(QJsonValue(attachement));
            }

            QJsonObject detail;
            for (int i=0; i<mTravelPersons.size(); i++) {
                traBusPersonInfo *info = mTravelPersons.at(i);

                detail.insert("addressBegin", info->getLeavePlace());             //(string)出发地点
                detail.insert("addressEnd", info->getArrivePlace());             //(string)到达地点
                detail.insert("billId", "");    //(string)表头单据id
                detail.insert("budgetSubjectId", info->getBudgetNumber());    //(string)预算科目id
                detail.insert("budgetSubjectName", info->getBudgetName());  //预算科目名称 string
                detail.insert("businessMattersId", "");  //业务事项(字典: dict_data_business_matters) string
                detail.insert("businessMattersName", "");          //(string)业务事项名称
                detail.insert("dateBegin", info->getLeaveDate());     //起始时间 string
                detail.insert("dateEnd", info->getReturnDate());       //终止时间 string
                detail.insert("days", info->getDays());                   //出差天数 int
                detail.insert("feeDepartId", "");        //费用承担部门id string
                detail.insert("feeDepartName", info->getDepartment());        //费用承担部门id string
                detail.insert("fundsType", info->getBudgetType());           //经费类型(dict_fund_type) string
                detail.insert("id", "");                 //ID唯一标识 string
                detail.insert("level", "");                //职级id string
                detail.insert("moneyOther", info->getOtherFee());   //其他费用金额 number
                detail.insert("moneyStay", info->getStayFee());     //住宿费 number
                detail.insert("moneySum", info->getTotalFee());     //小计 number
                detail.insert("moneyTrans", info->getCityFee());    //城市间交通费 number
                detail.insert("otherMemo", "");          //其他费用说明 string
                detail.insert("people", info->getStaffNumber());  //(string)出差人员id
                detail.insert("peopleDepartId", "");    //(string)部门id
                detail.insert("peopleDepartName", info->getDepartment());   //(string)部门名称
                detail.insert("peopleName", info->getStaffName());  //(string)出差人员名称
                detail.insert("peoplePost", "");            //职务 string
                detail.insert("peopleType", "");         //人员类别 string
                detail.insert("projectCode", QString::number(info->getBudgetNumber()));  //预算项目编码 string
                detail.insert("projectId", QString::number(info->getBudgetNumber()));          //预算项目id string
                detail.insert("projectName", info->getBudgetName());    //预算项目名称 string
                detail.insert("subsidyDays", info->getDays());     //(integer)补贴天数
                detail.insert("subsidyEating", info->getLunchSubsidy());    //(number)伙食补贴
                detail.insert("subsidyTrans", info->getTraSubsidy());       //(number)交通补贴

                detailList.append(QJsonValue(detail));
            }

            QJsonObject pay;
            for (int i=0 ; i<mPayItemList.size() ; i++) {
                payItemInfo *info = mPayItemList.at(i);

                pay.insert("bankAddress", info->getBankName());   //(string)开户银行
                pay.insert("bankName", info->getBank());      //(string)所属银行
                pay.insert("bankNo", info->getBankcardNumber());     //(string)银行账号
                pay.insert("billId", "");        //(string)单据id
                pay.insert("busiCardNo", "");    //(string)公务卡卡号
                pay.insert("cardDate", info->getCardDate().toString()); //(string)刷卡日期
                pay.insert("cardNo", info->getBankcardNumber());    //(string)银行卡卡号
                pay.insert("checkNum", "");      //(string)支票号
                pay.insert("collectPeopleId", "");   //(string)收款人/单位id
                pay.insert("collectPeopleName", info->getPayeeName());     //(string)收款人/单位名称
                pay.insert("counterPartName", info->getDepartment());       //(string)对方单位名称
                pay.insert("id", "");            //(string)
                pay.insert("jkCode", "");        //(string)借款单编号
                pay.insert("jkId", "");          //(string)借款单id
                pay.insert("memo", info->getRemark());          //(string)备注
                pay.insert("money", info->getAccount());         //(number)金额
                pay.insert("moneyBack", 0);     //(number)应归还金额
                pay.insert("moneyCard", 0);     //(number)刷卡金额
                pay.insert("moneyLoan", 0);     //(number)借款金额
                pay.insert("moneyOffset", 0);   //(number)冲借款金额
                pay.insert("payeeCity", info->getCity());     //(string)收款人省市
                pay.insert("paymethod", info->getStrType());     //(string)支付方式

                payList.append(QJsonValue(pay));
            }

            object.insert("attachementList", QJsonValue(attachementList));
            object.insert("audit", 0);	//单据审核状态 int
            object.insert("auditFlag", "");	//是否查询审批标识 string
            object.insert("auditState", 1);	//审批状态 int
            object.insert("billCount", billList.size());	//单据张数 int
            object.insert("billDate", local.toString("yyyy-MM-dd"));	//填报日期 string
            object.insert("check", false);	//数据是否完整 bool
            object.insert("checkNum", "");	//支票号 string
            object.insert("code", "");      //单据编号 string
            object.insert("departId", chargeDepartment);	//经办人部门 string
            object.insert("detailList", QJsonValue(detailList));
            object.insert("id", "");		//唯一标识 string
            object.insert("isElecInvoice", "");//是否有电子发票号 string
            object.insert("moneyBack", 0);	//还款金额合计 number
            object.insert("moneyCheck", 0);	//手续费 number
            object.insert("moneyOffset", 0);	//冲款金额合计
            object.insert("moneyReim", 0);	//报销金额合计
            object.insert("moneyReissue", 0);//应补发金额合计
            object.insert("operator", chargeManName);	//经办人 string
            object.insert("payDate", local.toString("yyyy-MM-dd"));	//支付日期 string
            object.insert("payList", QJsonValue(payList));

            QString peoples = mTravelPersons.at(1)->getStaffName();
            for (int i=1; i<mTravelPersons.size(); i++) {
                peoples = peoples + ",";
                peoples = peoples + mTravelPersons.at(i)->getStaffName();
            }
            object.insert("peoples", peoples);

            object.insert("period", "");	//单据所属区间 string
            object.insert("procInsId", "");	//流程实例Id string
            object.insert("projectType", "");	//项目类型 string
            object.insert("reportPerson", 0);//填报人 int
            object.insert("submitAudit", 0);	//是否提交 Int
            object.insert("use", ui->busiReason->toPlainText());		//出差事由string

            document1.setObject(object);
            QByteArray bytes = document1.toJson(QJsonDocument::Compact);
            QString strBytes(bytes);

            qDebug() << strBytes;
        }
    }
    else if (expenseType == 14) {
        if (totalPayAccount > totalPerAccount) {
            QMessageBox::warning(this, "支付总金额问题", "支付总金额不得超过人员补贴总金额", QMessageBox::Ok);
        } else {
            QJsonObject object;
            QJsonArray attachementList;
            QJsonArray detailList;
            QJsonArray payList;
            QJsonDocument document1;

            QJsonObject attachement;
            for (int i=0; i<billList.size(); i++) {
                BillCheck bill = billList.at(i);

                attachement.insert("billid", bill.getBillid());            //单据id string
                attachement.insert("extend", bill.getBillexpenseid());      //拓展名 string
                attachement.insert("id", bill.getBillid());                //ID唯一标示
                attachement.insert("invoiceNum", bill.getBusid());        //发票号 string
                attachement.insert("invoiceType", "1");                     //发票类型(0纸制1电子) string
                attachement.insert("name", bill.getBillattachmenttitle());  //附件名称 string
                attachement.insert("path", bill.getBillrealpath());         //附件路径 string
                attachement.insert("reviewpath", bill.getBillrealpath());   //预览路径 string
                attachement.insert("size", 0);                           //附件大小 int
                attachement.insert("swfpath", "");                          //swf格式路径 string
                attachement.insert("type", "");                             //附件类型 string

                attachementList.append(QJsonValue(attachement));
            }

            QJsonObject detail;
            for (int i=0; i<mAbroadPersons.size(); i++) {
                abroadPersonInfo *info = mAbroadPersons.at(i);

                detail.insert("billConut", billList.size());             //单据张数 int
                detail.insert("billId", "");             //单据id string
                detail.insert("budgetSubjectId", info->getBudgetNum());    //预算科目id string
                detail.insert("budgetSubjectName", info->getBudgetName());  //预算科目名称 string
                detail.insert("businessMattersId", "");  //业务事项(字典: dict_data_business_matters) string
                detail.insert("cityBegin", info->getLeaveCity());          //出发城市 string
                detail.insert("cityEnd", info->getArriveCity());            //到达城市 string
                detail.insert("dateBegin", info->getLeaveDate());     //起始时间 string
                detail.insert("dateEnd", info->getReturnDate());       //终止时间 string
                detail.insert("days", info->getDays());                   //出国天数 int
                detail.insert("departId", "");           //部门id string
                detail.insert("feeDepartId", "");        //费用承担部门id string
                detail.insert("fundsType", info->getBudgetType());           //经费类型(dict_fund_type) string
                detail.insert("id", "");                 //ID唯一标识 string
                detail.insert("level", "");                //职级id string
                detail.insert("moneyMeal", 0);              //伙食费外币金额 number
                detail.insert("moneyMiscellaneou", 0);      //公杂费外币金额 number
                detail.insert("moneyOther", 0);             //其他费外币金额 number
                detail.insert("moneyRmbMeal", 0);           //伙食费人民币 number
                detail.insert("moneyRmbMiscellaneou", 0);   //公杂费人民币 number
                detail.insert("moneyRmbOther", 0);          //其他费人民币 number
                detail.insert("moneyRmbStay", 0);           //住宿费人民币 number
                detail.insert("moneyRmbSum", 0);            //人民币小计 number
                detail.insert("moneyRmbTraffic", 0);        //交通费人民币 number
                detail.insert("moneyStandardMeal", 0);      //伙食费标准币种金额 number
                detail.insert("moneyStandardMiscellaneou", 0);    //公杂费标准币种金额 number
                detail.insert("moneyStandardOther", 0);     //其他费标准币种金额 number
                detail.insert("moneyStandardStay", 0);      //住宿费标准币种金额 number
                detail.insert("moneyStandardSum", 0);       //标准币种小计 number
                detail.insert("moneyStandardTraffic", 0);   //交通费标准币种金额 number
                detail.insert("moneyStay", 0);              //住宿费外币金额 number
                detail.insert("moneyTraffic", 0);           //交通费外币金额 number
                detail.insert("otherMemo", "说明");          //其他费用说明 string
                detail.insert("peopleCode", "123");         //员工工号 string
                detail.insert("peopleId", "123");           //员工id string
                detail.insert("peoplePost", "");            //职务 string
                detail.insert("peopleType", "秘书");         //人员类别 string
                detail.insert("projectCode", "123");        //预算项目编码 string
                detail.insert("projectId", "123");          //预算项目id string
                detail.insert("projectName", "项目1");       //预算项目名称 string
                detail.insert("rateMeal", 2);               //伙食费币种汇率 number
                detail.insert("rateMiscellaneou", 1.2);     //公杂费币种汇率 number
                detail.insert("rateOther", 0.5);            //其他费币种汇率 number
                detail.insert("rateStandard", 1.2);         //标准币种汇率 number
                detail.insert("rateStay", 1.2);             //住宿费币种汇率 number
                detail.insert("rateTraffic", 1.2);          //交通费币种汇率 number
                detail.insert("typeMeal", "美元");           //伙食费币种 string
                detail.insert("typeMiscellaneou", "美元");   //公杂费币种 string
                detail.insert("typeOther", "美元");          //其他费币种 string
                detail.insert("typeStandard", "美元");       //到达地标准币种 string
                detail.insert("typeStay", "美元");           //住宿费币种 string
                detail.insert("typeTraffic", "美元");         //交通费币种 string

                detailList.append(QJsonValue(detail));
            }

            QJsonObject pay;
            for (int i=0 ; i<mPayItemList.size() ; i++) {
                payItemInfo *info = mPayItemList.at(i);

                pay.insert("bankAddress", info->getBankName());   //(string)开户银行
                pay.insert("bankName", info->getBank());      //(string)所属银行
                pay.insert("bankNo", info->getBankcardNumber());     //(string)银行账号
                pay.insert("billId", "");        //(string)单据id
                pay.insert("busiCardNo", "");    //(string)公务卡卡号
                pay.insert("cardDate", info->getCardDate().toString()); //(string)刷卡日期
                pay.insert("cardNo", info->getBankcardNumber());    //(string)银行卡卡号
                pay.insert("checkNum", "");      //(string)支票号
                pay.insert("collectPeopleId", "");   //(string)收款人/单位id
                pay.insert("collectPeopleName", info->getPayeeName());     //(string)收款人/单位名称
                pay.insert("counterPartName", info->getDepartment());       //(string)对方单位名称
                pay.insert("id", "");            //(string)
                pay.insert("jkCode", "");        //(string)借款单编号
                pay.insert("jkId", "");          //(string)借款单id
                pay.insert("memo", info->getRemark());          //(string)备注
                pay.insert("money", info->getAccount());         //(number)金额
                pay.insert("moneyBack", 0);     //(number)应归还金额
                pay.insert("moneyCard", 0);     //(number)刷卡金额
                pay.insert("moneyLoan", 0);     //(number)借款金额
                pay.insert("moneyOffset", 0);   //(number)冲借款金额
                pay.insert("payeeCity", info->getCity());     //(string)收款人省市
                pay.insert("paymethod", info->getStrType());     //(string)支付方式

                payList.append(QJsonValue(pay));
            }

            object.insert("attachementList", QJsonValue(attachementList));
            object.insert("audit", 1);	//单据审核状态 int
            object.insert("auditFlag", "yes");	//是否查询审批标识 string
            object.insert("auditState", 1);	//审批状态 int
            object.insert("billCount", 1);	//单据张数 int
            object.insert("billDate", "20190901");	//单据日期 string
            object.insert("check", true);	//数据是否完整 bool
            object.insert("checkNum", "123");	//支票号 string
            object.insert("code", "123");	//单据编号 string
            object.insert("departId", "秘书部");	//经办人部门 string
            object.insert("detailList", QJsonValue(detailList));
            object.insert("id", "only");		//唯一标识 string
            object.insert("isElecInvoice", "yes");//是否有电子发票号 string
            object.insert("moneyBack", 111111);	//还款金额合计 number
            object.insert("moneyCheck", 111);	//手续费 number
            object.insert("moneyOffset", 1111);	//冲款金额合计
            object.insert("moneyReim", 111111);	//报销金额合计
            object.insert("moneyReissue", 111111);//应补发金额合计
            object.insert("operator", "张三");	//经办人 string
            object.insert("payDate", "20190901");	//支付日期 string
            object.insert("payList", QJsonValue(payList));
            object.insert("period", "1-100");	//单据所属区间 string
            object.insert("procInsId", "123");	//流程实例Id string
            object.insert("projectType", "未命名");	//项目类型 string
            object.insert("reportPerson", 123);//填报人 int
            object.insert("submitAudit", 1);	//是否提交 Int
            object.insert("use", "出国");		//出国事由string

            document1.setObject(object);
            QByteArray bytes = document1.toJson(QJsonDocument::Compact);
            QString strBytes(bytes);

            qDebug() << strBytes;
        }
    }
}

void MainWindow::on_commitPayBtn_clicked()
{
    if (expenseType == 11) {
        if (totalPayAccount > totalReimAccount) {
            QMessageBox::warning(this, "支付总金额问题", "支付总金额不得超过报销总金额", QMessageBox::Ok);
        } else {

        }
    }
    else if (expenseType == 12 || expenseType == 14) {
        if (totalPayAccount > totalPerAccount) {
            QMessageBox::warning(this, "支付总金额问题", "支付总金额不得超过人员补贴总金额", QMessageBox::Ok);
        } else {

        }
    }
}


