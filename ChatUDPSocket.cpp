#include "ChatUDPSocket.h"

extern quint16 ChatPort;
ChatUDPSocket::ChatUDPSocket(QObject *parent) :
    QObject(parent)
{
    mUdpSocket = new QUdpSocket(this);
    mUdpSocket->bind(QHostAddress::Any, ChatPort);
    connect(mUdpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}
ChatUDPSocket::~ChatUDPSocket(){
    delete mUdpSocket;
}

//ChatUDPSocket::ChatUDPSocket(const QHostAddress &listeningHost, QObject *parent) :
//    QObject(parent)
//{
//    this->listeningHost = listeningHost;
//}

void ChatUDPSocket::send(ChatMessage *message){
    QByteArray data;
    data.append("request:");
    data.append(message->getUuid().toString()).append(":");
    data.append(message->getContent());
    mUdpSocket->writeDatagram(data, message->getReceiver(), ChatPort);
    mUdpSocket->flush();
}

void ChatUDPSocket::readyRead (){
    QByteArray buffer;
    buffer.resize(mUdpSocket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    mUdpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    qDebug() << "message from : " << sender.toString();
    qDebug() << "message port : " << senderPort;
    qDebug() << "message : " << buffer;

    QList<QByteArray> packList = buffer.split(':');

    QString pType(packList[0].trimmed().toLower());
    QUuid pUuid(packList[1]);
    QString pContent(packList[2]);

    qDebug() << pType;

    if(pType == "request") { // request message
        QByteArray data;
        data.append("response:");
        data.append(pUuid.toString()).append(":");
        data.append("accepted");
        mUdpSocket->writeDatagram(data, sender, ChatPort);
        mUdpSocket->flush();
        emit receiveSuccess(pContent);
    } else if (pType == "response") { // response message
        if(pContent.trimmed().toLower() == "accepted") {
            emit sendSuccess(pUuid);
        }
    }
}
