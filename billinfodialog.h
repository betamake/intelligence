#ifndef BILLINFODIALOG_H
#define BILLINFODIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QLabel>


namespace Ui {
class billinfodialog;
}

class billinfodialog : public QDialog
{
    Q_OBJECT

public:
    explicit billinfodialog(QWidget *parent = 0);
    ~billinfodialog();
    void showimg(QTableWidgetItem *img);
    void showbill(QString billpath);

private:
    Ui::billinfodialog *ui;

};

#endif // BILLINFODIALOG_H
