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
            content = QString(contentArr);
        }
    }
}

ChatMessage::ChatMessage(const QUuid &uuid, Type type, const QUuid &userUuid, const QString &content, QObject *parent) :
    QObject(parent)
{
    setUuid(uuid);
    setType(type);
    setSenderUuid(userUuid);
    setContent(content);
}

ChatMessage::ChatMessage(const ChatMessage& cm)
{
    uuid = cm.uuid;
    senderUuid = cm.senderUuid;
    type = cm.type;
    content = cm.content;
}

ChatMessage::ChatMessage(Type type, const QUuid &senderUuid, const QString &content, QObject *parent) :
    QObject(parent)
{
    setUuid(QUuid::createUuid());
    setType(type);
    setSenderUuid(senderUuid);
    setContent(content);
}

QString ChatMessage::toString(){
    return QString("HEAD:%1:%2:%3;%4;")
            .arg(uuid.toString())
            .arg((int)type)
            .arg(senderUuid.toString())
            .arg(content);
}

QString ChatMessage::getContent(){
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

void ChatMessage::setContent(const QString& content){
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
