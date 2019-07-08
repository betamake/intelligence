#ifndef IDCARDTHREAD_H
#define IDCARDTHREAD_H

#include <QObject>
#include <QThread>
#include<QMutex>
#include "Information.h"

class IdCardThread : public QThread
{
    Q_OBJECT
public:
    explicit IdCardThread(QObject *parent = 0);
    //    void stop();
    void run();
    ~IdCardThread();
public slots:
    void stopImmediately();

protected:
signals:
//    void emitIdInfo(Information info, bool exitCode);//身份证读取成功就停止当前线程
    void emitIdInfo(Information info, int exitCode);//身份证读取成功就停止当前线程


private:
    QString idInfo;
//    volatile bool isChecked;
    QMutex m_lock;
    bool m_isCanRun;

};

#endif // IDCARDTHREAD_H
