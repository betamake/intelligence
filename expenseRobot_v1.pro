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
HEADERS += abroadexp.h \
           abroadexpense.h \
           billcheck.h \
           billinfodialog.h \
           busiexp.h \
           busiexpense.h \
           ClientSocket.h \
           commonutils.h \
           costexp.h \
           costexpense.h \
           databaseutils.h \
           expensehead.h \
           facemangedialog.h \
           idcardthread.h \
           Information.h \
           Instructions.h \
           mainwindow.h \
           payinfo.h \
           qaudiolevel.h \
           Serial.h \
           ServerSocket.h \
           Stream.h \
           Timestamp.h \
#           ui_billinfodialog.h \
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
    scanthread.h
FORMS += billinfodialog.ui \
         facemangedialog.ui \
         mainwindow.ui \
         userinfodialog.ui \
    scandialog.ui
SOURCES += abroadexp.cpp \
           abroadexpense.cpp \
           billcheck.cpp \
           billinfodialog.cpp \
           busiexp.cpp \
           busiexpense.cpp \
           ClientSocket.cpp \
           commonutils.cpp \
           costexp.cpp \
           costexpense.cpp \
           databaseutils.cpp \
           expensehead.cpp \
           facemangedialog.cpp \
           idcardthread.cpp \
           Information.cpp \
           Instructions.cpp \
           main.cpp \
           mainwindow.cpp \
           payinfo.cpp \
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
    scanthread.cpp
