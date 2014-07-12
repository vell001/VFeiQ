#include "UdpService.h"

UdpService::UdpService(QObject *parent) :
    QObject(parent),
    chatPort(9514)
{
    listen();
}
UdpService::UdpService(quint16 chatPort, QObject *parent) :
    QObject(parent),
    chatPort(chatPort)
{
    listen();
}

void UdpService::listen(){
    mUdpSocket = new QUdpSocket(this);
    mUdpSocket->bind(QHostAddress::Any, chatPort);
    connect(mUdpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

UdpService::~UdpService(){
    mUdpSocket->close();
    delete mUdpSocket;
}

void UdpService::send(ChatMessage &message, const QHostAddress &receiverIp){
    mUdpSocket->writeDatagram(message.toString().toUtf8(), receiverIp, chatPort);
    mUdpSocket->flush();
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

//    connect(message, SIGNAL(parseError(QString)), this, SIGNAL(receiveError(QString)));

    emit received(senderIp, senderPort, message);
}
