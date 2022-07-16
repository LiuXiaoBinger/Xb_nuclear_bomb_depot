#-------------------------------------------------
#
# Project created by QtCreator 2022-07-09T22:06:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xiaoBcelin
TEMPLATE = app
#
INCLUDEPATH +=./mediator
INCLUDEPATH +=./net
include(./audioapi/audioapi.pri)
include(./VideoApi/videoapi.pri)
INCLUDEPATH += ./audioapi/
INCLUDEPATH += ./VideoApi/
#
LIBS += -lpthread libwsock32 libws2_32
LIBS += -lpthread libMswsock libMswsock
SOURCES += main.cpp\
        mychatdialog.cpp \
    net/TcpClienNet.cpp \
    mediator/INetMediator.cpp \
    mediator/TcpClienMediator.cpp \
    chatdialog.cpp \
    ckernel.cpp \
    logindialog.cpp \
    userlten.cpp

HEADERS  += mychatdialog.h \
    net/INet.h \
    net/pack.h \
    net/TcpClienNet.h \
    mediator/INetMediator.h \
    mediator/TcpClienMediator.h \
    chatdialog.h \
    ckernel.h \
    logindialog.h \
    userlten.h

FORMS    += mychatdialog.ui \
    chatdialog.ui \
    logindialog.ui \
    userlten.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    VideoApi/videoapi.pri
