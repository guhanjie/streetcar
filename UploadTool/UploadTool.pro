#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T11:18:24
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UploadTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    uploadworker.cpp

HEADERS  += mainwindow.h \
    uploadworker.h

FORMS    += mainwindow.ui
