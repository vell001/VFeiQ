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
    connect(mUdpService, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(recived(QHostAddress,quint16,ChatMessage)));
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

void ChatService::recived(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(message.getType() == ChatMessage::Request) { // request message
        ChatMessage respMes(message.getUuid(), ChatMessage::Response, SettingUtil::getUtil()->getSender()->getUuid(), "accepted");
        send(respMes, senderIp);
        emit receiveSuccess(senderIp, senderPort, message);
    } else if (message.getType() == ChatMessage::Response) { // response message
        if(message.getContent().trimmed().toLower() == "accepted") {
            emit sendSuccess(message.getUuid());
        } else {
            emit sendError(message.getUuid(), QString("unaccepted"));
        }
    }
}
