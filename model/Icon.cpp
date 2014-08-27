#include "Icon.h"

Icon::Icon(QObject *parent) :
    QObject(parent)
{
}
Icon::Icon(const QString &iconXMLStr, const QHostAddress &senderIp, QObject *parent) :
    QObject(parent)
{
//    <icon id="" fileFullName=""/>
    QDomDocument iconDoc;
    QString errorMsg;
    iconDoc.setContent(iconXMLStr, &errorMsg);
    if(!errorMsg.isEmpty()) {
        emit parseError(errorMsg);
    }

    QDomElement iconE = iconDoc.firstChildElement();
    uuid = QUuid(iconE.attribute("id"));
    fileInfo = QFileInfo(iconE.attribute("fileFullName"));
    icon = QIcon(fileInfo.filePath());
}

Icon::Icon(const QFileInfo &fileInfo, const QUuid &uuid, QObject *parent) :
    QObject(parent),
    fileInfo(fileInfo),
    uuid(uuid)
{
    icon = QIcon(fileInfo.filePath());
}

QString Icon::toXMLString(int indent){
//    <icon id="" fileFullName=""/>
    QDomDocument iconDoc;
    QDomElement userE = iconDoc.createElement("icon");
    userE.setAttribute("id", uuid.toString());
    userE.setAttribute("fileFullName", fileInfo.filePath());

    iconDoc.appendChild(userE);
    return iconDoc.toString(indent);
}

QHash<QUuid, Icon *> *Icon::parseIconsFromXMLString(const QString &XMLStr){
    QHash<QUuid, Icon *> *icons = new QHash<QUuid, Icon *>;
    QDomDocument messageDoc;
    QString errorMsg;
    messageDoc.setContent(XMLStr, &errorMsg);
    if(!errorMsg.isEmpty()) {
        return icons;
    }
    QDomElement msgE = messageDoc.firstChildElement();

    Icon *icon;
    for(int i=0; i<msgE.childNodes().count(); i++) {
        QDomNode file = msgE.childNodes().item(i);
        if(!file.isElement()) continue;
        QDomElement iconE = file.toElement();
        icon = new Icon();

        icon->setUuid(QUuid(iconE.attribute("id")));
        icon->setFileInfo(QFileInfo(iconE.attribute("fileFullName")));
    }
    return icons;
}

QString Icon::IconsToXMLString(const QHash<QUuid, Icon *> &icons, int indent){
    QDomDocument messageDoc;
    QDomElement iconsE = messageDoc.createElement("icons");
    messageDoc.appendChild(iconsE);
    foreach (Icon *icon, icons.values()) {
        QDomElement msgE = messageDoc.createElement("icon");

        msgE.setAttribute("id", icon->getUuid().toString());
        msgE.setAttribute("fileFullName", icon->getFileInfo().filePath());
        iconsE.appendChild(msgE);
    }
    return messageDoc.toString(indent);
}

QUuid Icon::getUuid(){
    return uuid;
}

QIcon Icon::getIcon(){
    return icon;
}

QFileInfo Icon::getFileInfo(){
    return fileInfo;
}

void Icon::setUuid(const QUuid &uuid){
    this->uuid = uuid;
}

void Icon::setIcon(const QIcon &icon){
    this->icon = icon;
}

void Icon::setFileInfo(const QFileInfo &fileInfo){
    this->fileInfo = fileInfo;
}
