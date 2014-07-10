/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**
**********************************************************************/
#ifndef CHATUDPSOCKET_H
#define CHATUDPSOCKET_H

#include <QObject>
#include <QUdpSocket>
#include <QUuid>
#include <QList>
#include "model/ChatMessage.h"
class ChatUDPSocket : public QObject
{
    Q_OBJECT
public:
    explicit ChatUDPSocket(QObject *parent = 0);
    ~ChatUDPSocket();
//    explicit ChatUDPSocket(const QHostAddress &listeningHost, QObject *parent = 0);
    void send(ChatMessage *message);
signals:
    void sendError(QString errorMessage);
    void sendSuccess(QUuid messageUuid);
    void receiveSuccess(QString content);
public slots:
    void readyRead ();
private:
    QUdpSocket *mUdpSocket;
    QHostAddress listeningHost;
//    quint16 ChatPort;
};

#endif // CHATUDPSOCKET_H
