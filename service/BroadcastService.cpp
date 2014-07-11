#include "BroadcastService.h"

BroadcastService::BroadcastService(QObject *parent) :
    QObject(parent)
{
    this->mUdpService = new UdpService(9703, this);
    listen();
}
BroadcastService::BroadcastService(quint16 chatPort, QObject *parent) :
    QObject(parent)
{
    this->mUdpService = new UdpService(chatPort, this);
    listen();
}

void BroadcastService::listen(){
    connect(mUdpService, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SIGNAL(received(QHostAddress,quint16,ChatMessage)));
}

BroadcastService *BroadcastService::getService(){
    static BroadcastService mService;
    return &mService;
}

BroadcastService::~BroadcastService(){
    delete mUdpService;
}

void BroadcastService::send(ChatMessage &message, const QHostAddress &receiverIp){
    mUdpService->send(message, receiverIp);
}
