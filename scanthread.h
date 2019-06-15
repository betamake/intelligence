#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QObject>

class ScanThread : public QObject
{
    Q_OBJECT
public:
    explicit ScanThread(QObject *parent = 0);

signals:

public slots:
};

#endif // SCANTHREAD_H