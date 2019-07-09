#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include<QTableWidget>
#include<QMediaRecorder>
#include "voicecheck.h"
#include "databaseutils.h"
#include "qaudiolevel.h"
#include "user.h"
#include "commonutils.h"
#include "billcheck.h"
#include <QString>
#include "idcardthread.h"
#include "userinfodialog.h"
#include "billinfodialog.h"
#include "facemangedialog.h"
#include "scandialog.h"
#include "windows/insertpersonneldialog.h"
#include "windows/addpaydialog.h"
#include "windows/itemViews/paymethodsitem.h"
#include "windows/itemViews/billitem.h"
#include "payinfomanager.h"
//#include "networkthread.h"
//#include "networkhandler.h"
#include <QMap>
#include "turing.h"
//#include "ClientSocket.h"
#include "Serial.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Instructions.h"
//#include "ServerSocket.h"
#include <pthread.h>

#include <QPaintEvent>
#include <QTimer>
#include<QKeyEvent>
#include "scanthread.h" //扫面线程
#include "costbaseinfo.h"//处理费用报销基本信息
#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;
class QTimer;
class QByteArray;
class QNetworkAccessManager;
class QNetworkReply;
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;
class QNetworkRequest;
class QMediaPlayer;
class QAudioLevel;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QNetworkRequest getHttpRequest(QString url);
    QByteArray  Image_To_Base64(QString ImgPath);
    void toLastStep();
    void toNextStep();
    void toCurrentPage(int currentIndex);
    void instructionExp(QString instruction);
    void initMedia();
    void printFile(QString dataFileName, QByteArray fdata);
    bool eventFilter(QObject * watched, QEvent * event);//QLabel的所有消息都会经过此函数

    QString address;
    //展示欢迎页
    void paintEvent(QPaintEvent*);
    void idCardRead();//身份证读取
    void showUserInfo(User user);//全局信息展示
    QString log_id; //记录人脸注册，返回成功值
    void scanBillstart();//扫描票据

    void deletePhoto(QString uid);//删除人脸库指定uid人脸
    bool VerifyNumber(QString str);
    //清空内容
//    void deleteBillInput(); //清空票据录入内容
//    void deleteTravelContent(); //清空差旅报内容
//    void deleteCostContent(); //清空费用报销内容
//    void deleteAbroadContent(); //清空出国报销内容
//    void deletePayInforn(); //清空支付信息
//    void deleteInfornConfirm();//清空信息确认内容
    void deleteAccountInfo(); //清空账号登录信息
    void deleteFace(); //回收人脸识别资源
    void deleteAccountReg(); //清空注册信息.
    void isShowguiInforn(); //显示报销流程引导
    void noShowguiInforn(); //不显示报销流程引导
//报销流程处理函数
    void costbaseRead() ;//读取费用报销基本信息



private:
    void startRecord();
    void stopRecord();
    void setRecord();
    void sendPlayText(QByteArray text);
//    void on_faceRegBtn_clicked();

signals:
//    void costDone(); //发票识别完成发送信号.
//    void busiDone(); //差旅报销
//    void abroadDone();//出国报销

private slots:
    void on_faceBtn_clicked();
    void on_idBtn_clicked();
    void faceCheck();//人脸检测
    void faceReply(QNetworkReply * reply);
    void faceRegReply(QNetworkReply * reply);
    void billReply(QNetworkReply * reply);
    void showBillimg(QTableWidgetItem * img);//显示票据图片
    void voiceReply(QNetworkReply * reply);
    void playReply(QNetworkReply * reply);
    void sendPhoto(int id,QImage image);
    void faceDelReply(QNetworkReply * reply);
    void photoRegister(int id,QImage image);
    void on_lastStepBtn_clicked();
    void on_nextStepBtn_clicked();
    void on_mainWidget_currentChanged(int arg1);
    void on_costBtn_clicked();
    void on_busiBtn_clicked();
    void on_abroadBtn_clicked();
//    void on_busiBodyAddBtn_clicked();
//    void on_busiBodyDelBtn_clicked();
    void on_fileChooseBtn_clicked();
    void on_continueExpenseBtn_clicked();
    void on_logoutBtn_clicked();
//    void on_confirmBtn_clicked();
    void processBuffer(const QAudioBuffer&);
    void playChanged();
    void on_carmBtn_clicked();
    void on_LogBtn_clicked();
    void on_RegBtn_2_clicked();
    void on_accountBtn_clicked();
    void on_user_loginBtn_clicked();
    void on_RegAcountBtn_clicked();
    //void on_costBodyAddBtn_clicked();
    //void on_costBodyDelBtn_clicked();
//    void on_pushButton_clicked();
//    void on_pushButton_2_clicked();
    void dealIdCardReply(Information,int);
    void on_idLogin_clicked();
    void on_idCheckIgnoreBtn_clicked();
    void on_idCheckBtn_clicked();
    void showRegPhoto(QString);//注册页面显示图片
    void on_pushButton_3_clicked();
    void dealGetIdCheck(User user, int currentIndex);

    void on_firstBtn_clicked();
    void on_faceManBtn_clicked();
    void on_scanBillbtn_clicked();
    void onLocalThreadDestroy(QObject* obj);//一个成员变量记录正在运行的线程
    void on_carmSkipBtn_clicked();

    void on_talkBtn_clicked();
    //void on_answerBtn_clicked();
    void on_expenseBtn_clicked();
    void billScanRelpy();//处理扫面后的图片.
    //表格信息处理
    //void dealCost(); //处理发票信息
//    void dealBusi(); //处理差旅信息
//    void dealAbroad();// 处理出国信息;

    void on_applyButton_clicked();

    void on_advisoryButton_clicked();

    void on_costBaseButton_clicked();

    void on_costScanButton_clicked();

    void on_costItemButton_clicked();

    void on_costPayButton_clicked();

    void on_costAgainButton_clicked();

    void on_addPersonnel_clicked();

    void on_addPayInfoBtn_clicked();

    void addPayInfoItem(payItemInfo *info);      //添加支付信息，参数后续添加

    void openPayInfoItem(payItemInfo *info, int index);        //打开要修改的支付信息的窗口
    void modifyPayInfoItem(payItemInfo *info);      //修改支付信息


private:
    void setBasePage(int expenseType);


private:
    Ui::MainWindow *ui;
    QCamera *camera;
    QCameraViewfinder * viewfinder;
    QCameraImageCapture * imageCapture;
    QCameraImageCapture * imageCaptureReg;
//    QList<QByteArray> list;
    QNetworkAccessManager * manager;
    QNetworkAccessManager * manager2; //人脸删除的
    bool isFaceExits;//查找人脸库是否存在人脸 true:存在
    QAudioRecorder *audioRecorder;
    QAudioProbe *probe;
    QMediaPlayer * player;
    QList<QAudioLevel*> audioLevels;
    int facetime;
    int currentIndex;
    int expenseType;//报销类型
    bool busiTalble;
    //判断是否选择此报销类型;
    bool costType =false;
    bool busiType =false;
    bool abroadType = false;
    //表格
//    QTableWidget *bsTable;
//    QTableWidget *costTable;
//    QTableWidget *abdTable;
//    QTableWidget *blchkTable;
//    QTableWidget *confirmTable;
    QTableWidget *table;
//    BusiExpense busi;//差旅费用
//    costExpense cost;//费用报销单
//    abroadExpense abroad;//出国报销单
    int expenseTypeId;
//    PayInfo pay;
    BillCheck bill; // 票据数据
    BillCheck billinfo;
//    busiExp busiexp;
//    abroadExp abdexp;
//    costExp costexp;
//    ExpenseHead exphead;

    int recordNum;//音频样本数量
    int recordLow;//低音音频样本数量
    int recordHigh;//高音样本数量
    bool recordSend;//是否发送语音指令
    bool playStatus;//是否播放过音频文件
    QString regFilePath;
    QString uname;
    QString billpath;
    QString billdir;
    QStringList bldir;
    QStringList blpath;
    QString picName;//保存的图片名称
    QString billname;//票据附件名
    //发票相关
    QString billCode; //参考票据识别接口
    QString billNumber;
    QString fbillDate;
    QString billGoods;
    QString billPrice;
    QString billCheckcode;
    int costRowcount;
    // 火车票相关
    QString trainPerson;//参照火车票返回接口
    QString trainDate;
    QString trainStart;
    QString trainPrice;
    QString trainEnd;
    int busiRowcount;
    //飞机票相关
    int abroadRowcount;

    //数据库相关
    databaseUtils database;
    CommonUtils commonutils;

    //多线程
    void idCardThreadStop();
    Information idCardInformation;
    int idCheckId;//身份证识别id 1身份证注册,2身份证登录
    void scanBillThreadStop();//停止扫描仪线程
    int isScan;
    QString scanBillpath;

    //用户信息弹框
    UserInfoDialog* userInfoDialog;
    FaceMangeDialog* faceMangeDialog;

    bool isFaceOk;//check if face ok
    int idFace;//1表示人脸注册， 2表示人脸检测
    bool isIdcardChecked;
    User regUser;//注册用户信息
    int regCode;//注册流程，1账号注册， 2人脸注册, 3身份认证 ，1和2必须
    User loginUser;//当前在线用户
    int loginStatus;//登录状态 0未登录 1人脸 2身份证 3账号
    Information loginInformation;//当前用户身份信息
    billinfodialog* billInfoDialog;//显示票据的弹框
    scanDialog *scanInfoDialog;


    //黄梦君，添加的成员的标记，勿删
    insertPersonnelDialog *insertPerDialog;
    addPayDialog *addPayDlg;
    int modifyItemIndex;        //要修改的支付信息的序号，从0开始，对应的是在list中的行数

    QByteArray getPixmapData(QString filePath,QImage image);
    QString avaterFilePath;

    IdCardThread* m_currentRunLoaclThread;
    IdCardThread* m_thread;

    //Qobject请求网络
//    QTimer m_heart;
//    QTimer *m_timer;
    QTimer *timer;

//    NetworkThread* m_obj;
//    QThread* m_objThread;

//    NetworkHandler *m_network;
    Turing *turing;
    //扫描线程
    ScanThread *myScanThread;
    //
    costBase costBinfo;

};

#endif // MAINWINDOW_H
