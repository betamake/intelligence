#include "userinfodialog.h"
#include "ui_userinfodialog.h"
#include <QMessageBox>
#include <QDebug>
UserInfoDialog::UserInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("个人中心"));
    //隐藏所有控件
    ui->idCardNum3_line->hide();
    ui->userName_line->hide();
    ui->phoneNum_line->hide();
    ui->idlabelIsCheck->hide();

    ui->cancleBtn->hide();
    ui->submitBtn->hide();
    ui->changeBtn->show();//弹框时只显示修改按钮
    ui->getIdCheckBtn->hide();
    //connect()
}

UserInfoDialog::~UserInfoDialog()
{
    delete ui;
}
/**
* @brief        展示用户数据
* @author        胡帅成
* @date        2018-09-11
*/
void UserInfoDialog::showUserInfo(User user, Information information, int loginStatus,int idCheckId)
{
    //展示用户数据
    this->user = user;

    qDebug() << "当前登录用户："<< user.toString();
    qDebug() << "当前登录身份信息："<<information.toString();

    //判断用户信息是否存在
//    if(!user.getUsername().isEmpty())
    if(loginStatus!=0)
    {
        //隐藏身份信息控件
        ui->idlabelNumber3->hide();
        ui->idCardNum3->hide();
        ui->idCardNum3_line->hide();
        ui->idlabelName3->hide();
        ui->idName_3->hide();
        ui->idName3_line->hide();
        ui->idlabelBirth3->hide();
        ui->idBirth3->hide();
        ui->idBirth3_line->hide();
        ui->idlabelNation3->hide();
        ui->idNation3->hide();
        ui->idNation3_line->hide();
        ui->idlabelSex3->hide();
        ui->idSex3->hide();
        ui->idSex3_line->hide();
        ui->idlabelAddress3->hide();
        ui->idAddress3->hide();
        ui->idAddress3_line->hide();
        ui->idlabelNewAdd3->hide();
        ui->idNewAdd3->hide();
        ui->idlabelNewAdd3_line->hide();
        ui->idlSign3->hide();
        ui->idSign3_line->hide();
        ui->idlabelSign3->hide();

        //存在
        ui->userName->setText(user.getUsername());
        ui->phoneNum->setText(user.getPhoneNumber());
        //展示用户头像
        if(!user.getAvater().isNull())
        {
            QPixmap avater;
            avater.loadFromData(user.getAvater().toByteArray(),"jpg");
            ui->idAvater->setPixmap(avater);
            ui->idAvater->setScaledContents(true);
        }else
        {
            ui->idAvater->setText("当前无头像");
        }

        //判断身份信息是否存在
//        if(!user.getIdCard().isEmpty())
        if(idCheckId == 2)
        {
            //身份信息存在，则显示已认证，展示身份信息数据
            ui->idlabelNumber3->show();
            ui->idCardNum3->show();
            ui->idCardNum3->setText(QString::fromStdString(information.getIdNumber()));

            ui->idlabelName3->show();
            ui->idName_3->show();
            ui->idName_3->setText(QString::fromStdString(information.getName()));

            ui->idlabelBirth3->show();
            ui->idBirth3->show();
            ui->idBirth3->setText(QString::fromStdString(information.getBirthday()));

            ui->idlabelNation3->show();
            ui->idNation3->show();
            ui->idNation3->setText(QString::fromStdString(information.getNation()));

            ui->idlabelSex3->show();
            ui->idSex3->show();
            ui->idSex3->setText(QString::fromStdString(information.getSex()));

            ui->idlabelSign3->show();
            ui->idlSign3->show();
            ui->idlSign3->setText(QString::fromStdString(information.getSigningOrganization()));

            ui->idlabelAddress3->show();
            ui->idAddress3->show();
            ui->idAddress3->setText(QString::fromStdString(information.getAddress()));

            ui->idlabelNewAdd3->show();
            ui->idNewAdd3->show();
            ui->idNewAdd3->setText(QString::fromStdString(information.getNewestAddress()));
        }else
        {
            ui->idlabelIsCheck->setText(tr("当前用户未实名认证！"));
            ui->idlabelIsCheck->setAlignment(Qt::AlignCenter);
            ui->idlabelIsCheck->show();
            ui->getIdCheckBtn->show();
        }
    }
}

/**
* @brief         提交修改后的信息
* @author        胡帅成
* @date        2018-09-10
*/
void UserInfoDialog::on_submitBtn_clicked()
{
    QString username = ui->userName_line->text().trimmed();
    //    QString idCardNum = ui->idCardNum3_line->text().trimmed();
    QString phoneNum = ui->phoneNum_line->text().trimmed();

    if(username!=""  || phoneNum!="" )
    {
        //        ui->phoneNum_line->setText(phoneNum);
        //        ui->idCardNum_line->setText(idCardNum);
        //        ui->userName_line->setText(username);
        //插入数据库
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("插入成功！"));
        this->close();
    }
    else
    {
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("数据不能为空！"));
    }


}
/**
* @brief         取消修改
* @author        胡帅成
* @date        2018-09-11
*/
void UserInfoDialog::on_cancleBtn_clicked()
{
    //取消提交
    this->close();
}
/**
* @brief        修改用户信息
* @author        胡帅成
* @date        2018-09-10
*/
void UserInfoDialog::on_changeBtn_clicked()
{
    //隐藏修改按钮
    ui->changeBtn->hide();
    ui->submitBtn->show();
    ui->cancleBtn->show();

    //显示修改框
    ui->userName->hide();
    ui->userName_line->show();
    ui->userName_line->setText(user.getUsername());

    ui->phoneNum->hide();
    ui->phoneNum_line->show();
    ui->phoneNum_line->setText(user.getPhoneNumber());

    //已认证用户显示
    if(!user.getIdCard().isEmpty())
    {
//        ui->getIdCheckBtn->hide();//隐藏认证按钮
        ui->idCardNum3->show();
        ui->idlabelNumber3->show();
        //        ui->idCardNum3_line->show();
        ui->idCardNum3->setText(user.getIdCard());
//        ui->getIdCheckBtn->show();//显示认证信息
    }else
    {
        ui->idCardNum3->hide();
        ui->idlabelNumber3->hide();
        ui->idCardNum3_line->hide();
        ui->getIdCheckBtn->show();//显示认证信息
    }
}

/**
* @brief         去认证
* @author        胡帅成
* @date        2018-09-20
*/
void UserInfoDialog::on_getIdCheckBtn_clicked()
{
//    qDebug() << "去认证！" << user.toString();
    //跳转页面
    emit emitIdCheck(user,7);
    this->close();
}
