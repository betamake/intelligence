######################################################################
# Automatically generated by qmake (3.1) Wed Nov 7 15:57:54 2018
######################################################################

QT+=widgets
QT+=network
QT += sql
QT += multimedia
QT += multimediawidgets
QT += script
QT += serialport
QT += concurrent
QT += qml quick network xml
CONFIG += c++11
RESOURCES += qml.qrc
RESOURCES += image/image.qrc
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
# Default rules for deployment.
include(deployment.pri)

UI_DIR=./UI


TEMPLATE = app
TARGET = expenseRobot_v1

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += \
           billcheck.h \
           ClientSocket.h \
           commonutils.h \
           facemangedialog.h \
           idcardthread.h \
           Information.h \
           Instructions.h \
           mainwindow.h \
           qaudiolevel.h \
           Serial.h \
           ServerSocket.h \
           Stream.h \
           Timestamp.h \
#           ui_facemangedialog.h \
#           ui_mainwindow.h \
#           ui_userinfodialog.h \
           user.h \
           userinfodialog.h \
           voicecheck.h \
    networkthread.h \
    connectionpool.h \
    networkhandler.h  \
    turing.h \
    scandialog.h \
    scanthread.h \
    windows/itemViews/addPersonnel.h \
    windows/insertpersonneldialog.h \
    windows/itemViews/abroadpersonnel.h \
    common/commondefs.h \
    windows/itemViews/paymethodsitem.h \
    windows/addpaydialog.h \
    windows/itemViews/billitem.h \
    windows/itemViews/reimdetailitem.h \
    costbaseinfo.h \
    travelbaseinfo.h \
    database.h \
    managers/reimdetailmanager.h \
    managers/payinfomanager.h \
    managers/personnelmanager.h
    costbaseinfo.h
FORMS += \
         facemangedialog.ui \
         mainwindow.ui \
         userinfodialog.ui \
    scandialog.ui \
    windows/itemViews/addPersonnel.ui \
    windows/insertpersonneldialog.ui \
    windows/itemViews/abroadpersonnel.ui \
    windows/itemViews/paymethodsitem.ui \
    windows/addpaydialog.ui \
    windows/itemViews/billitem.ui \
    windows/itemViews/reimdetailitem.ui\
    windows/addpaydialog.ui
SOURCES += \
           billcheck.cpp \
           ClientSocket.cpp \
           commonutils.cpp \
           facemangedialog.cpp \
           idcardthread.cpp \
           Information.cpp \
           Instructions.cpp \
           main.cpp \
           mainwindow.cpp \
           qaudiolevel.cpp \
           Serial.cpp \
           ServerSocket.cpp \
           Stream.cpp \
           Timestamp.cpp \
           user.cpp \
           userinfodialog.cpp \
           voicecheck.cpp \
    networkthread.cpp \
    connectionpool.cpp \
    networkhandler.cpp \
    turing.cpp \
    scandialog.cpp \
    scanthread.cpp \
    windows/itemViews/addPersonnel.cpp \
    windows/insertpersonneldialog.cpp \
    windows/itemViews/abroadpersonnel.cpp \
    common/commondefs.cpp \
    windows/itemViews/paymethodsitem.cpp \
    windows/addpaydialog.cpp \
    windows/itemViews/billitem.cpp \
    windows/itemViews/reimdetailitem.cpp \
    costbaseinfo.cpp \
    travelbaseinfo.cpp \
    database.cpp \
    managers/reimdetailmanager.cpp \
    managers/payinfomanager.cpp \
    managers/personnelmanager.cpp
