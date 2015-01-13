#-------------------------------------------------
#
# Project created by QtCreator 2014-12-02T15:02:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vlcplayer
TEMPLATE = app


INCLUDEPATH += . ..\vlcplaywindow

win32 {
    LIBS += -L..\vlcplaywindow\debug
    LIBS += -lvlcplaywindow
}else{
    LIBS += -L..\vlcplaywindow\debug
    LIBS += -lvlcplaywindow
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
