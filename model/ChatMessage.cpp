#include "ChatMessage.h"

ChatMessage::ChatMessage(const QByteArray &mesStr, QObject *parent) :
    QObject(parent)
{
    if(!mesStr.startsWith("HEAD:") || !mesStr.endsWith(";")){
        emit parseError("Incomplete message");
    } else {
        int splitIndex = mesStr.indexOf(';');
        if(splitIndex == -1 || splitIndex == mesStr.size()-1){
            emit parseError("Incomplete message structure");
        } else {
            QByteArray head = mesStr.left(splitIndex);
            QByteArray contentArr = mesStr.right(mesStr.size() - splitIndex - 1);
            contentArr = contentArr.left(contentArr.size()-1);

            QList<QByteArray> headList = head.split(':');
            if(headList.size() != 4) {
                emit parseError("Incomplete head structure");
            } else {
                setUuid(QUuid(headList[1]));
                setType(Type(headList[2].toInt()));
                setSenderUuid(QUuid(headList[3]));
            }
            content = contentArr;
        }
    }
}

ChatMessage::ChatMessage(const QUuid &uuid, Type type, const QUuid &senderUuid, const QByteArray &content, QObject *parent) :
    QObject(parent),
    uuid(uuid),
    senderUuid(senderUuid),
    content(content)
{
    this->type = type;
}

ChatMessage::ChatMessage(const ChatMessage& cm) :
    uuid(cm.uuid),
    senderUuid(cm.senderUuid),
    content(cm.content)
{
    type = cm.type;
}

ChatMessage &ChatMessage::operator=(const ChatMessage &cm){
    uuid = cm.uuid;
    senderUuid = cm.senderUuid;
    type = cm.type;
    content = cm.content;
    return *this;
}

ChatMessage::ChatMessage(Type type, const QUuid &senderUuid, const QByteArray &content, QObject *parent) :
    QObject(parent),
    senderUuid(senderUuid),
    content(content)
{
    setUuid(QUuid::createUuid());
    this->type = type;
}

QString ChatMessage::toString(){
    return QString("HEAD:%1:%2:%3;%4;")
            .arg(uuid.toString())
            .arg((int)type)
            .arg(senderUuid.toString())
            .arg(QString(content));
}

QByteArray ChatMessage::getContent(){
    return content;
}

QUuid ChatMessage::getUuid(){
    return uuid;
}
QUuid ChatMessage::getSenderUuid(){
    return senderUuid;
}
ChatMessage::Type ChatMessage::getType(){
    return type;
}

void ChatMessage::setContent(const QByteArray& content){
    this->content = content;
}

void ChatMessage::setUuid(const QUuid& uuid){
    this->uuid = uuid;
}

void ChatMessage::setSenderUuid(const QUuid &senderUuid){
    this->senderUuid = senderUuid;
}

void ChatMessage::setType(Type type){
    this->type = type;
}
