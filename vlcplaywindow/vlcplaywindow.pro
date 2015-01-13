#-------------------------------------------------
#
# Project created by QtCreator 2014-12-02T09:31:00
#
#-------------------------------------------------

TARGET = vlcplaywindow
TEMPLATE = lib

DEFINES += VLCPLAYWINDOW_LIBRARY

SOURCES += vlcplaywindow.cpp \
    Enums.cpp

HEADERS += vlcplaywindow.h\
        vlcplaywindow_global.h \
    SharedExportCore.h \
    Enums.h
LIBS += -L./lib
INCLUDEPATH += . ./vlc
win32 {
    LIBS += -llibvlc -llibvlccore
}else{
    LIBS += -lvlc -lvlccore
}

FORMS += \
    playwindow.ui
