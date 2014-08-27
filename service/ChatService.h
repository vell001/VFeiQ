/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**      `chatPost` default value is 9514
**********************************************************************/
#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <QObject>

#include "model/ChatMessage.h"
#include "service/UdpService.h"
#include "service/UserService.h"

class ChatService : public UdpService
{
    Q_OBJECT
public:
    static ChatService *getService();
signals:
    void sendError(QUuid messageUuid, QString errorMessage);
    void sendSuccess(QUuid messageUuid);
    void receivedMessage(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
    void receiveError(QString errorMessage);
    void receiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
public slots:
    void messageReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
private:
    explicit ChatService(QObject *parent = 0);
    User *sender;
    void listen();
};

#endif // CHATSERVICE_H
