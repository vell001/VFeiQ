#include "ChatRcord.h"

ChatRcord::ChatRcord(QObject *parent) :
    QObject(parent),
    time(QDateTime::currentDateTime())
{
}

ChatRcord::ChatRcord(ChatMessage message, QObject *parent) :
    QObject(parent),
    time(QDateTime::currentDateTime()),
    uuid(message.getUuid()),
    userUuid(message.getSenderUuid()),
    content(message.getContent())
{
    initSendOrReceive();
}

ChatRcord::ChatRcord(const QUuid &uuid, const QUuid &userUuid, const QString &content, QObject *parent) :
    QObject(parent),
    uuid(uuid),
    userUuid(userUuid),
    content(content),
    time(QDateTime::currentDateTime())
{
    initSendOrReceive();
}

void ChatRcord::initSendOrReceive(){
    myself = UserService.getMyself();
    if(userUuid == myself->getUuid()) {
        sendOrReceive = ChatRcord::Send;
    } else {
        sendOrReceive = ChatRcord::Receive;
    }
}

QUuid ChatRcord::getUuid(){
    return uuid;
}
QUuid ChatRcord::getUserUuid(){
    return userUuid;
}
QDateTime ChatRcord::getTime(){
    return time;
}
QString ChatRcord::getContent(){
    return content;
}
SendOrReceive ChatRcord::getSendOrReceive(){
    return sendOrReceive;
}

void ChatRcord::setUuid(QUuid uuid){
    this->uuid = uuid;
}

void ChatRcord::setUserUuid(QUuid userUuid){
    this->userUuid = userUuid;
}

void ChatRcord::setTime(QDateTime time){
    this->time = time;
}

void ChatRcord::setContent(QString content){
    this->content = content;
}

void ChatRcord::setSendOrReceive(SendOrReceive sendOrReceive){
    this->sendOrReceive = sendOrReceive;
}
