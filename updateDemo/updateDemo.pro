#-------------------------------------------------
#
# Project created by QtCreator 2015-05-22T11:57:57
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = updateDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    appconfig.cpp \
    dbutils.cpp \
    waitingdialog.cpp \
    loadingdialog.cpp

HEADERS  += mainwindow.h \
    appconfig.h \
    dbutils.h \
    waitingdialog.h \
    loadingdialog.h \
    quazip.h \
    ui_mainwindow.h \
    quazipfile.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

QMAKE_LFLAGS += -Wl,-rpath=$$PWD/

unix:!macx: LIBS += -L$$PWD/ -lquazip
unix:!macx: LIBS += -L$$PWD/ -lqsqloci

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
