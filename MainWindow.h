/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**
**********************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDebug>
#include <QList>
#include "ChatForm.h"
#include "service/BroadcastService.h"
#include "util/SettingUtil.h"
#include "model/User.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openChatForm(QModelIndex);
    void receiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message);

private:
    Ui::MainWindow *ui;
    BroadcastService *mBroadcastService;
    SettingUtil *mSettingUtil;
    ChatMessage *myselfMessage;
    QList<User> *mFriends;
    void updateContentsTreeWidget();
};

#endif // MAINWINDOW_H
