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
    mode = Mode(headE.attribute("mode").toInt());
    senderUuid = QUuid(headE.attribute("senderUuid"));
    contentType = ContentType(headE.attribute("contentType").toInt());
    createTime = QDateTime::fromString(headE.attribute("createTime"));
    content = contentE.text();
}

ChatMessage::ChatMessage(const QUuid &uuid, Mode mode, const QUuid &senderUuid,
                         const QString &content, ContentType contentType,
                         const QDateTime &createTime, QObject *parent) :
    QObject(parent),
    uuid(uuid),
    senderUuid(senderUuid),
    content(content),
    mode(mode),
    contentType(contentType),
    createTime(createTime)
{
}

ChatMessage::ChatMessage(const ChatMessage& cm) :
    uuid(cm.uuid),
    senderUuid(cm.senderUuid),
    content(cm.content),
    mode(cm.mode),
    contentType(cm.contentType),
    createTime(cm.createTime)
{
}

ChatMessage &ChatMessage::operator=(const ChatMessage &cm){
    uuid = cm.uuid;
    senderUuid = cm.senderUuid;
    mode = cm.mode;
    content = cm.content;
    contentType = cm.contentType;
    createTime = cm.createTime;
    return *this;
}

ChatMessage::ChatMessage(Mode mode, const QUuid &senderUuid, const QString &content,
                         ContentType contentType, const QDateTime &createTime, QObject *parent) :
    QObject(parent),
    senderUuid(senderUuid),
    content(content),
    mode(mode),
    contentType(contentType),
    uuid(QUuid::createUuid()),
    createTime(createTime)
{
}

QString ChatMessage::toString(){
    QDomDocument messageDoc;
    QDomElement msgE = messageDoc.createElement("message");
    QDomElement headE = messageDoc.createElement("HEAD");
    QDomElement contentE = messageDoc.createElement("content");
    QDomText contentT = messageDoc.createTextNode(content);

    headE.setAttribute("id", uuid.toString());
    headE.setAttribute("mode", (int)mode);
    headE.setAttribute("senderUuid", senderUuid.toString());
    headE.setAttribute("contentType", (int)contentType);
    headE.setAttribute("createTime", createTime.toString());

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
ChatMessage::ContentType ChatMessage::getContentType(){
    return contentType;
}
QDateTime ChatMessage::getCreateTime(){
    return createTime;
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

void ChatMessage::setMode(Mode mode){
    this->mode = mode;
}

void ChatMessage::setContentType(ContentType contentType){
    this->contentType = contentType;
}

void ChatMessage::setCreateTime(const QDateTime &createTime){
    this->createTime = createTime;
}
