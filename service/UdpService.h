#ifndef UDPSERVICE_H
#define UDPSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QUuid>
#include "model/ChatMessage.h"

class UdpService : public QObject
{
    Q_OBJECT
public:
    explicit UdpService(QObject *parent = 0);
    explicit UdpService(quint16 chatPort, QObject *parent = 0);
    ~UdpService();
    void send(ChatMessage &message, const QHostAddress &receiverIp);
signals:
    void received(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
    void serviceError(QString errorMesssage);
public slots:
    void readyRead();
private:
    QUdpSocket *mUdpSocket;
    quint16 chatPort;
    void startListen();
};

#endif // UDPSERVICE_H
