#include "ChatService.h"


ChatService::ChatService(QObject *parent) :
    UdpService(9514,parent)
{
    listen();
}

void ChatService::listen(){
    sender = UserService::getService()->getMyself();
    connect(this, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(messageReceived(QHostAddress,quint16,ChatMessage)));
    connect(this, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SIGNAL(receivedMessage(QHostAddress,quint16,ChatMessage)));
}

ChatService *ChatService::getService(){
    static ChatService service;
    return &service;
}

void ChatService::messageReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(message.getMode() == ChatMessage::Request) { // request message
        ChatMessage respMes(message.getUuid(), ChatMessage::Response, sender->getUuid(), "accepted");
        UdpService::send(respMes, senderIp);
        emit receiveSuccess(senderIp, senderPort, message);
    } else if (message.getMode() == ChatMessage::Response) { // response message
        if(message.getContent().trimmed().toLower() == "accepted") {
            emit sendSuccess(message.getUuid());
        } else {
            emit sendError(message.getUuid(), QString("unaccepted"));
        }
    }
}
