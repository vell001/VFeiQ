/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**
**********************************************************************/
#ifndef CHATFORM_H
#define CHATFORM_H

#include <QWidget>
#include "model/ChatMessage.h"
#include "service/ChatService.h"
#include <QListWidgetItem>
#include <QKeyEvent>
#include <model/User.h>
#include "util/SettingUtil.h"

namespace Ui {
class ChatForm;
}

class ChatForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatForm(const User &receiver, QWidget *parent = 0);
    ~ChatForm();

protected:
    void keyPressEvent(QKeyEvent *e);
private slots:
    void on_sendButton_clicked();
    void sendError(QUuid messageUuid, QString errorMessage);
    void sendSuccess(QUuid messageUuid);
    void receiveSuccess(ChatMessage message);

private:
    Ui::ChatForm *ui;
    ChatService *mChatService;
    User receiver;
    void sendMessage();
};

#endif // CHATFORM_H
