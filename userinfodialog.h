#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include "user.h"
#include "commonutils.h"
#include "Information.h"
namespace Ui {
class UserInfoDialog;
}

class UserInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfoDialog(QWidget *parent = 0);
    ~UserInfoDialog();
    void showUserInfo(User user, Information information,int loginStatus,int idCheckId);//显示用户数据
private slots:

    void on_submitBtn_clicked();

    void on_cancleBtn_clicked();

    void on_changeBtn_clicked();


    void on_getIdCheckBtn_clicked();
signals:
    void emitIdCheck(User user, int pageNum);//跳转认证页面
private:
    Ui::UserInfoDialog *ui;
    CommonUtils commonutils;
    User user;


};

#endif // USERINFODIALOG_H
