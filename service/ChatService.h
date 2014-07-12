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

class ChatService : public QObject
{
    Q_OBJECT
public:
    explicit ChatService(QObject *parent = 0);
    explicit ChatService(quint16 chatPort, QObject *parent = 0);
    static ChatService *getService();
    ~ChatService();
    void send(ChatMessage &message, const QHostAddress &receiverIp);
signals:
    void sendError(QUuid messageUuid, QString errorMessage);
    void sendSuccess(QUuid messageUuid);
    void receiveError(QString errorMessage);
    void receiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
public slots:
    void recived(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
private:
    UdpService *mUdpService;
    User *sender;
    void listen();
};

#endif // CHATSERVICE_H
