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
#include "ChatUDPSocket.h"
#include <QListWidgetItem>
#include <QKeyEvent>

namespace Ui {
class ChatForm;
}

class ChatForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatForm(QWidget *parent = 0);
    ~ChatForm();
protected:
    void keyPressEvent(QKeyEvent *e);
private slots:
    void on_sendButton_clicked();
    void sendError(QString errorMessage);
    void sendSuccess(QUuid messageUuid);
    void receiveSuccess(QString content);

private:
    Ui::ChatForm *ui;
    ChatUDPSocket *mChatUDPSocket;
    void sendMessage();
};

#endif // CHATFORM_H
