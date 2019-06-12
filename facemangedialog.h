#ifndef FACEMANGEDIALOG_H
#define FACEMANGEDIALOG_H

#include <QDialog>

namespace Ui {
class FaceMangeDialog;
}

class FaceMangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FaceMangeDialog(QWidget *parent = 0);
    ~FaceMangeDialog();

private:
    Ui::FaceMangeDialog *ui;
};

#endif // FACEMANGEDIALOG_H
