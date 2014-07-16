/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**
**********************************************************************/
#ifndef CHATFORM_H
#define CHATFORM_H

#include <QWidget>
#include "service/ChatService.h"
#include <QListWidgetItem>
#include <QKeyEvent>
#include <model/User.h>
#include <QList>
#include <QFontDialog>
#include <QColorDialog>
#include <QSettings>
#include "service/ChatRecordService.h"
#include "service/IconService.h"

namespace Ui {
class ChatForm;
}

class ChatForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatForm(User *receiver, QWidget *parent = 0);
    ~ChatForm();

protected:
    void keyPressEvent(QKeyEvent *e);
    virtual void closeEvent ( QCloseEvent * event );
private slots:
    void on_sendButton_clicked();
    void sendError(QUuid messageUuid, QString errorMessage);
    void sendSuccess(QUuid messageUuid);
    void receiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
    void on_closeButton_clicked();

    void on_fontButton_clicked();

    void on_colorButton_clicked();

    void on_facesButton_clicked();

signals:
    void closed(QUuid receiverUuid);
private:
    Ui::ChatForm *ui;
    ChatService *mChatService;
    User *sender;
    User *receiver;
    void sendMessage();

    QList<ChatRecord *> mChatRecords;
    void initForm();
    void updateChatRecordView();

    IconService *mIconService;

    void loadSetting();
    void saveSetting();
};

#endif // CHATFORM_H
