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
    dbutils.cpp

HEADERS  += mainwindow.h \
    appconfig.h \
    dbutils.h

FORMS    += mainwindow.ui