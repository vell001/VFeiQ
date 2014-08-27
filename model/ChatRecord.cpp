#include "ChatRecord.h"

ChatRecord::ChatRecord(QObject *parent) :
    QObject(parent),
    time(QDateTime::currentDateTime())
{
}

ChatRecord::ChatRecord(ChatMessage message, QObject *parent) :
    QObject(parent),
    time(message.getCreateTime()),
    uuid(message.getUuid()),
    userUuid(message.getSenderUuid()),
    content(message.getContent()),
    isRead(NotRead)
{
    initSendOrReceive();
}

ChatRecord::ChatRecord(const QUuid &uuid, const QUuid &userUuid, const QString &content, QObject *parent) :
    QObject(parent),
    uuid(uuid),
    userUuid(userUuid),
    content(content),
    time(QDateTime::currentDateTime()),
    isRead(NotRead)
{
    initSendOrReceive();
}

void ChatRecord::initSendOrReceive(){
    myself = UserService::getService()->getMyself();
    if(userUuid == myself->getUuid()) {
        sendOrReceive = ChatRecord::Send;
    } else {
        sendOrReceive = ChatRecord::Receive;
    }
}
ChatRecord::ChatRecord(const ChatRecord &cr):
    uuid(cr.uuid),
    userUuid(cr.userUuid),
    time(cr.time),
    content(cr.content),
    sendOrReceive(cr.sendOrReceive),
    isRead(cr.isRead)
{
}

ChatRecord &ChatRecord::operator=(const ChatRecord &cr){
    uuid=cr.uuid;
    userUuid=cr.userUuid;
    time=cr.time;
    content=cr.content;
    sendOrReceive=cr.sendOrReceive;
    isRead=cr.isRead;
}

bool ChatRecord::operator<(const ChatRecord &cr){
    return time < cr.time;
}

QUuid ChatRecord::getUuid(){
    return uuid;
}
QUuid ChatRecord::getUserUuid(){
    return userUuid;
}
QDateTime ChatRecord::getTime(){
    return time;
}
QString ChatRecord::getContent(){
    return content;
}
ChatRecord::SendOrReceive ChatRecord::getSendOrReceive(){
    return sendOrReceive;
}
ChatRecord::IsRead ChatRecord::getIsRead(){
    return isRead;
}

void ChatRecord::setUuid(QUuid uuid){
    this->uuid = uuid;
}

void ChatRecord::setUserUuid(QUuid userUuid){
    this->userUuid = userUuid;
}

void ChatRecord::setTime(QDateTime time){
    this->time = time;
}

void ChatRecord::setContent(QString content){
    this->content = content;
}

void ChatRecord::setSendOrReceive(SendOrReceive sendOrReceive){
    this->sendOrReceive = sendOrReceive;
}

void ChatRecord::setIsRead(IsRead isRead){
    this->isRead = isRead;
}
