#-------------------------------------------------
#
# Project created by QtCreator 2012-03-26T22:52:22
#
#-------------------------------------------------

QT       += core gui network

TARGET = client
TEMPLATE = app

SOURCES += main.cpp\
    gamewindow.cpp \
    lobbygui.cpp \
    tableinfo.cpp \
    playerinfo.cpp \
    pokerclient.cpp \
    model.cpp \
    startwindow.cpp

HEADERS  += \
    gamewindow.h \
    lobbygui.h \
    tableinfo.h \
    playerinfo.h \
    pokerclient.h \
    model.h \
    startwindow.h \
    constants.h

FORMS    += \
    ui/gamewindow.ui \
    ui/lobbygui.ui \
    ui/dialog.ui \
    ui/startwindow.ui

RESOURCES += \
    resources.qrc
