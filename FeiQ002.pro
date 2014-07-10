#-------------------------------------------------
#
# Project created by QtCreator 2014-07-07T15:12:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FeiQ002
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ChatForm.cpp \
    ChatUDPSocket.cpp \
    model/ChatMessage.cpp \
    model/Friend.cpp

HEADERS  += MainWindow.h \
    ChatForm.h \
    ChatUDPSocket.h \
    model/ChatMessage.h \
    model/Friend.h

FORMS    += MainWindow.ui \
    ChatForm.ui

RESOURCES += \
    skins.qrc \
    images.qrc
