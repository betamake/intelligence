#include "mainwindow.h"
#include <QApplication>
#include<QSplitter>
#include<QListWidget>
#include<QTextCodec>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <turing.h>
//#include <frminput.h>
//#include "databaseutils.h"
//#pragma execution_character_set("utf-8")
// 解决中文乱码的问题.
//#if _MSC_VER >= 1600
//#pragma execution_character_set("utf-8")
//#endif


int main(int argc, char *argv[])
{
//    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    MainWindow *mainWindow = new MainWindow(0);
//    frmInput::Instance ()->hide ();
    mainWindow->show ();
//    qmlRegisterType<Turing>("io.qt.turing", 1, 0, "Turning");

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return a.exec();
}
