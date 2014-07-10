#include "ChatMessage.h"

ChatMessage::ChatMessage(QObject *parent) :
    QObject(parent)
{
    this->uuid = QUuid::createUuid();
}

ChatMessage::ChatMessage(const QString &content, const QHostAddress &receiver, QObject *parent) :
    QObject(parent)
{
    this->uuid = QUuid::createUuid();
    setContent(content);
    setReceiver(receiver);
}


QString ChatMessage::getContent(){
    return content;
}

QHostAddress ChatMessage::getReceiver(){
    return receiver;
}

QUuid ChatMessage::getUuid(){
    return uuid;
}

void ChatMessage::setContent(const QString& content){
    this->content = content;
}

void ChatMessage::setReceiver(const QHostAddress& receiver){
    this->receiver = receiver;
}

void ChatMessage::setUuid(const QUuid& uuid){
    this->uuid = uuid;
}
