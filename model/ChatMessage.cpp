#include "ChatMessage.h"

ChatMessage::ChatMessage(const QString &mesStr, QObject *parent) :
    QObject(parent)
{
    QDomDocument messageDoc;
    QString errorMsg;
    messageDoc.setContent(mesStr, &errorMsg);
    if(!errorMsg.isEmpty()) {
        emit parseError(errorMsg);
    }
    QDomElement msgE = messageDoc.firstChildElement();
    QDomElement headE = msgE.elementsByTagName("HEAD").at(0).toElement();
    QDomElement contentE = msgE.elementsByTagName("content").at(0).toElement();

    uuid = QUuid(headE.attribute("id"));
    mode = Mode(headE.attribute("type").toInt());
    senderUuid = QUuid(headE.attribute("senderUuid"));
    content = contentE.text();
/*
    if(!mesStr.startsWith("HEAD:") || !mesStr.endsWith(";")){
        emit parseError("Incomplete message");
    } else {
        int splitIndex = mesStr.indexOf(';');
        if(splitIndex == -1 || splitIndex == mesStr.size()-1){
            emit parseError("Incomplete message structure");
        } else {
            QString head = mesStr.left(splitIndex);
            QString contentArr = mesStr.right(mesStr.size() - splitIndex - 1);
            contentArr = contentArr.left(contentArr.size()-1);

            QStringList headList = head.split(':');
            if(headList.size() != 4) {
                emit parseError("Incomplete head structure");
            } else {
                setUuid(QUuid(headList[1]));
                setType(Type(headList[2].toInt()));
                setSenderUuid(QUuid(headList[3]));
            }
            content = contentArr;
        }
    }*/
}

ChatMessage::ChatMessage(const QUuid &uuid, Mode type, const QUuid &senderUuid, const QString &content, QObject *parent) :
    QObject(parent),
    uuid(uuid),
    senderUuid(senderUuid),
    content(content)
{
    this->mode = type;
}

ChatMessage::ChatMessage(const ChatMessage& cm) :
    uuid(cm.uuid),
    senderUuid(cm.senderUuid),
    content(cm.content)
{
    mode = cm.mode;
}

ChatMessage &ChatMessage::operator=(const ChatMessage &cm){
    uuid = cm.uuid;
    senderUuid = cm.senderUuid;
    mode = cm.mode;
    content = cm.content;
    return *this;
}

ChatMessage::ChatMessage(Mode type, const QUuid &senderUuid, const QString &content, QObject *parent) :
    QObject(parent),
    senderUuid(senderUuid),
    content(content)
{
    setUuid(QUuid::createUuid());
    this->mode = type;
}

QString ChatMessage::toString(){
    QDomDocument messageDoc;
    QDomElement msgE = messageDoc.createElement("message");
    QDomElement headE = messageDoc.createElement("HEAD");
    QDomElement contentE = messageDoc.createElement("content");
    QDomText contentT = messageDoc.createTextNode(content);

    headE.setAttribute("id", uuid.toString());
    headE.setAttribute("type", (int)mode);
    headE.setAttribute("senderUuid", senderUuid.toString());

    messageDoc.appendChild(msgE);
    msgE.appendChild(headE);
    msgE.appendChild(contentE);
    contentE.appendChild(contentT);

    return messageDoc.toString(-1);
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
ChatMessage::Mode ChatMessage::getMode(){
    return mode;
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

void ChatMessage::setMode(Mode type){
    this->mode = type;
}
