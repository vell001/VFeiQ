#include "ChatService.h"


ChatService::ChatService(QObject *parent) :
    QObject(parent)
{
    this->mUdpService = new UdpService(9514, this);
    listen();
}
ChatService::ChatService(quint16 chatPort, QObject *parent) :
    QObject(parent)
{
    this->mUdpService = new UdpService(chatPort, this);
    listen();
}

void ChatService::listen(){
    connect(mUdpService, SIGNAL(receiveError(QString)), this, SIGNAL(receiveError(QString)));
    connect(mUdpService, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)), this, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)));
    connect(mUdpService, SIGNAL(sendError(QUuid,QString)), this, SIGNAL(sendError(QUuid,QString)));
    connect(mUdpService, SIGNAL(sendSuccess(QUuid)), this, SIGNAL(sendSuccess(QUuid)));
}

ChatService *ChatService::getService(){
    static ChatService mChatService;
    return &mChatService;
}

ChatService::~ChatService(){
    delete mUdpService;
}

void ChatService::send(ChatMessage &message, const QHostAddress &receiverIp){
    mUdpService->send(message, receiverIp);
}
