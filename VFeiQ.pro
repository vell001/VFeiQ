#-------------------------------------------------
#
# Project created by QtCreator 2014-07-07T15:12:15
#
#-------------------------------------------------

QT       += core gui network xml webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = VFeiQ
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ChatForm.cpp \
    model/ChatMessage.cpp \
    model/FriendSet.cpp \
    service/ChatService.cpp \
    model/User.cpp \
    service/UdpService.cpp \
    service/UserService.cpp \
    model/ChatRecord.cpp \
    service/ChatRecordService.cpp \
    service/IconService.cpp \
    MessageDialog.cpp \
    model/FileSender.cpp \
    model/FileReceiver.cpp \
    model/FileMessage.cpp \
    SharedFilesDialog.cpp \
    service/StorageService.cpp \
    service/FileShareService.cpp \
    SetShareFilesDialog.cpp \
    model/Icon.cpp \
    SettingDialog.cpp \
    service/FileMessageService.cpp \
    service/UserInfoService.cpp \
    GamesDialog.cpp \
    ScreenshotsWidget.cpp \
    model/Image.cpp \
    FacesDialog.cpp \
    WeatherDialog.cpp

HEADERS  += MainWindow.h \
    ChatForm.h \
    model/ChatMessage.h \
    model/FriendSet.h \
    service/ChatService.h \
    model/User.h \
    service/UdpService.h \
    service/UserService.h \
    model/ChatRecord.h \
    service/ChatRecordService.h \
    service/IconService.h \
    MessageDialog.h \
    model/FileSender.h \
    model/FileReceiver.h \
    model/FileMessage.h \
    SharedFilesDialog.h \
    service/StorageService.h \
    service/FileShareService.h \
    SetShareFilesDialog.h \
    model/Icon.h \
    SettingDialog.h \
    service/FileMessageService.h \
    service/UserInfoService.h \
    GamesDialog.h \
    ScreenshotsWidget.h \
    model/Image.h \
    FacesDialog.h \
    WeatherDialog.h

FORMS    += MainWindow.ui \
    ChatForm.ui \
    MessageDialog.ui \
    SharedFilesDialog.ui \
    SetShareFilesDialog.ui \
    SettingDialog.ui \
    GamesDialog.ui \
    FacesDialog.ui \
    WeatherDialog.ui

RESOURCES += \
    skins.qrc \
    images.qrc \
    normalUserIcons.qrc \
    faces.qrc

#DESTDIR = ./bin

#temp-directory sources
#OBJECTS_DIR = ./.objtmp
#MOC_DIR = ./.moctmp
#UI_DIR  += ./.uitmp
