/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**      `chatPost` default value is 9514
**********************************************************************/
#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QUuid>
#include <QList>
#include "model/ChatMessage.h"
#include "model/User.h"
#include "util/SettingUtil.h"

class ChatService : public QObject
{
    Q_OBJECT
public:
    static ChatService *getService();
    ~ChatService();
    void send(ChatMessage &message, const QHostAddress &receiverIp);
signals:
    void sendError(QUuid messageUuid, QString errorMessage);
    void sendSuccess(QUuid messageUuid);
    void receiveError(QString errorMessage);
    void receiveSuccess(ChatMessage message);
public slots:
    void readyRead ();
private:
    QUdpSocket *mUdpSocket;
    quint16 chatPort;
    void listen();
    explicit ChatService(QObject *parent = 0);
};

#endif // CHATUDPSOCKET_H
