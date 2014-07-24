#include "UdpService.h"

UdpService::UdpService(QObject *parent) :
    QObject(parent),
    chatPort(9514)
{
    startListen();
}
UdpService::UdpService(quint16 chatPort, QObject *parent) :
    QObject(parent),
    chatPort(chatPort)
{
    startListen();
}

void UdpService::startListen(){
    mUdpSocket = new QUdpSocket(this);
    if(!mUdpSocket->bind(QHostAddress::Any, chatPort)){
        emit serviceError("UdpSocket bind error");
    } else {
        connect(mUdpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
}

UdpService::~UdpService(){
    mUdpSocket->close();
    delete mUdpSocket;
}

void UdpService::send(ChatMessage &message, const QHostAddress &receiverIp){
    mUdpSocket->writeDatagram(message.toString().toUtf8(), receiverIp, chatPort);
    mUdpSocket->flush();
}

void UdpService::broadcast(ChatMessage &message){
    send(message, QHostAddress("255.255.255.255"));
}

void UdpService::readyRead (){
    QByteArray buffer;
    buffer.resize(mUdpSocket->pendingDatagramSize());

    QHostAddress senderIp;
    quint16 senderPort;
    mUdpSocket->readDatagram(buffer.data(), buffer.size(), &senderIp, &senderPort);

    qDebug() << "message from : " << senderIp.toString();
    qDebug() << "message port : " << senderPort;
    qDebug() << "message : " << buffer;
    qDebug() << "";

    ChatMessage message(QString::fromUtf8(buffer));

    connect(&message, SIGNAL(parseError(QString)), this, SIGNAL(serviceError(QString)));

    emit received(senderIp, senderPort, message);
}
