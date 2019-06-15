#include "scanthread.h"

ScanThread::ScanThread(QObject *parent) : QThread(parent)
{

}
void ScanThread::run ()
{
    system ("scanimage --mode color --resolution 600 --format jpeg >bill.jpg ");
    system ("convert bill.jpg finalbill.jpg");
    emit isDone ();
}
