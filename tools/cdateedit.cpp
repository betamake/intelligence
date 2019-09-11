#include "cdateedit.h"

class CDateEdit::Private {
public:
    Private(CDateEdit* qq) : q(qq), null(false), nullable(false) {}

    CDateEdit* const q;

    bool null;
    bool nullable;

    void setNull(bool n) {
        null = n;
        if (null) {
            QLineEdit *edit = q->findChild<QLineEdit*>("qt_spinbox_lineedit");
            if (!edit->text().isEmpty()) {
                edit->clear();
            }
        }
    }
};

CDateEdit::CDateEdit(QWidget *parent) :
QDateEdit(parent), d(new Private(this))
{
}


QDateTime CDateEdit::dateTime() const
{
    if (d->nullable && d->null) {
        return QDateTime();
    }
    else {
        return QDateEdit::dateTime();
    }
}


QDate CDateEdit::date() const
{
    if (d->nullable && d->null) {
        return QDate();
    }
    else {
        return QDateEdit::date();
    }
}


QTime CDateEdit::time() const
{
    if (d->nullable && d->null) {
        return QTime();
    }
    else {
        return QDateEdit::time();
    }
}



void CDateEdit::setDateTime(const QDateTime &dateTime)
{
    if (d->nullable && !dateTime.isValid()) {
        d->setNull(true);
    }
    else {
        d->setNull(false);
        QDateEdit::setDateTime(dateTime);
    }
}


void CDateEdit::setDate(const QDate &date)
{
    if (d->nullable && !date.isValid()) {
        d->setNull(true);
    }
    else {
        d->setNull(false);
        QDateEdit::setDate(date);
    }
}

void CDateEdit::setTime(const QTime &time)
{
    if (d->nullable && !time.isValid()) {
        d->setNull(true);
    }
    else {
        d->setNull(false);
        QDateEdit::setTime(time);
    }
}

bool CDateEdit::isNullable() const
{
    return d->nullable;
}

void CDateEdit::setNullable(bool enable)
{
    d->nullable = enable;

    update();
}

QSize CDateEdit::sizeHint() const
{
    const QSize sz = QDateEdit::sizeHint();
    return QSize(sz.width() + 3, sz.height());
}

QSize CDateEdit::minimumSizeHint() const
{
    const QSize sz = QDateEdit::minimumSizeHint();
    return QSize(sz.width() + 3, sz.height());
}

void CDateEdit::showEvent(QShowEvent *event)
{
    QDateEdit::showEvent(event);
    d->setNull(d->null);
}

void CDateEdit::resizeEvent(QResizeEvent *event)
{
    QDateEdit::resizeEvent(event);
}


void CDateEdit::paintEvent(QPaintEvent *event)
{
    d->setNull(d->null);
    QDateEdit::paintEvent(event);

}

void CDateEdit::keyPressEvent(QKeyEvent *event)
{
    if (d->nullable &&
        (event->key() >= Qt::Key_0) &&
        (event->key() <= Qt::Key_9) &&
        d->null) {
        setDateTime(QDateTime::currentDateTime());
    }
    if (event->key() == Qt::Key_Tab && d->nullable && d->null) {
        QAbstractSpinBox::keyPressEvent(event);
        return;
    }
    if (event->key() == Qt::Key_Backspace && d->nullable){
        QLineEdit *edit = this->findChild<QLineEdit *>("qt_spinbox_lineedit");
        if (edit->selectedText() == edit->text()) {
            setDateTime(QDateTime());
            event->accept();
            return;
        }
    }

    QDateEdit::keyPressEvent(event);
}


void CDateEdit::mousePressEvent(QMouseEvent *event)
{
    bool saveNull = d->null;
    QDateEdit::mousePressEvent(event);
    if (d->nullable && saveNull && calendarWidget()->isVisible()) {
        setDateTime(QDateTime::currentDateTime());
    }
}

bool CDateEdit::focusNextPrevChild(bool next)
{
    if (d->nullable && d->null){
        return QAbstractSpinBox::focusNextPrevChild(next);
    }
    else {
        return QDateEdit::focusNextPrevChild(next);
    }
}

QValidator::State CDateEdit::validate(QString &input, int &pos) const
{
    if (d->nullable && d->null){
        return QValidator::Acceptable;
    }
    return QDateEdit::validate(input, pos);
}
