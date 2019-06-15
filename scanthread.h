#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QThread>
class ScanThread : public QThread
{
    Q_OBJECT
public:
    explicit ScanThread(QObject *parent = 0);
    void run();

signals:
    void isDone();

public slots:
};

#endif // SCANTHREAD_H
