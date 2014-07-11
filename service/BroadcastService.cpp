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
    connect(mUdpService, SIGNAL(receiveError(QString)), this, SIGNAL(receiveError(QString)));
    connect(mUdpService, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)), this, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)));
    connect(mUdpService, SIGNAL(sendError(QUuid,QString)), this, SIGNAL(sendError(QUuid,QString)));
    connect(mUdpService, SIGNAL(sendSuccess(QUuid)), this, SIGNAL(sendSuccess(QUuid)));
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
