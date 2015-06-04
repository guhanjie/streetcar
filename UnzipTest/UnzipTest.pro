#-------------------------------------------------
#
# Project created by QtCreator 2015-06-03T15:18:48
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = UnzipTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lquazip
else:unix: LIBS += -L$$PWD/ -lquazip

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
