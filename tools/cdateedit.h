#ifndef CDATEEDIT_H
#define CDATEEDIT_H


#include <QtCore>
#include <QWidget>
#include <QDateEdit>
#include <QStyle>
#include <QPushButton>
#include <QLineEdit>
#include <QStyleOptionSpinBox>
#include <QKeyEvent>
#include <QCalendarWidget>

class CDateEdit : public QDateEdit
{
    Q_OBJECT

public:
    explicit CDateEdit(QWidget *parent = 0);

    QDateTime dateTime() const;
    QDate date() const;
    QTime time() const;

    bool isNullable() const;
    void setNullable(bool enable);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
        void mousePressEvent(QMouseEvent *event);
    bool focusNextPrevChild(bool next);
    QValidator::State validate(QString &input, int &pos) const;

public slots:
    void setDateTime(const QDateTime &dateTime);
    void setDate(const QDate &date);
    void setTime(const QTime &time);

private:
    Q_DISABLE_COPY(CDateEdit)
    class Private;
    friend class Private;
    Private* d;

};

#endif // CDATEEDIT_H
