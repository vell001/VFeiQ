#ifndef BROADCASTSERVICE_H
#define BROADCASTSERVICE_H

#include <QObject>

#include "service/UdpService.h"
#include "model/User.h"
#include "model/ChatMessage.h"

class BroadcastService : public QObject
{
    Q_OBJECT
public:
    explicit BroadcastService(QObject *parent = 0);
    explicit BroadcastService(quint16 chatPort, QObject *parent = 0);
    static BroadcastService *getService();
    static BroadcastService *getService(quint16 chatPort);
    ~BroadcastService();
    void send(ChatMessage &message, const QHostAddress &receiverIp);

signals:
    void received(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
public slots:
protected:
    UdpService *mUdpService;
    void listen();
};

#endif // BROADCASTUDPSOCKET_H
