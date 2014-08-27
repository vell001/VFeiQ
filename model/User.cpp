#include "User.h"

User::User(QObject *parent) :
    QObject(parent)
{
    this->uuid = QUuid::createUuid();
}

User::User(const QHostAddress &ip, const QString &name, QObject *parent) :
    QObject(parent),
    ip(ip),
    name(name)
{
    this->uuid = QUuid::createUuid();
}

User::User(const QString &userXMLStr, QObject *parent) :
    QObject(parent)
{
//    <user id="[uuid]" ip="[ip]" name="[name]" logTime="[logTime]" info="[info]" status="[status]" iconData="[iconData]" />
    QDomDocument userDoc;
    QString errorMsg;
    userDoc.setContent(userXMLStr, &errorMsg);
    if(!errorMsg.isEmpty()) {
        emit parseError(errorMsg);
    }

    QDomElement userE = userDoc.firstChildElement();
    uuid = QUuid(userE.attribute("id"));
    ip = QHostAddress(userE.attribute("ip"));
    name = userE.attribute("name");
    logTime = QDateTime::fromString(userE.attribute("logTime"));
    info = userE.attribute("info");
    status = (Status) userE.attribute("status").toInt();

    icon = getIconFromCompressData(userE.attribute("iconData"));
}

User::User(const User &user)
{
    uuid = user.uuid;
    ip = user.ip;
    name = user.name;
    icon = user.icon;
    logTime = user.logTime;
    info = user.info;
    status = user.status;
}

User &User::operator=(const User &user)
{
    uuid = user.uuid;
    ip = user.ip;
    name = user.name;
    icon = user.icon;
    logTime = user.logTime;
    info = user.info;
    status = user.status;
    return *this;
}

bool User::operator== ( const User & user ) const{
    if(uuid == user.uuid
       && ip == user.ip
       && name == user.name
       && getCompressDataFromIcon(icon) == getCompressDataFromIcon(user.icon)
       && logTime == user.logTime
       && info == user.info
       && status == user.status) {
        return true;
    } else {
        return false;
    }
}

bool User::operator!= ( const User & user ) const{
    if(*this == user) {
        return false;
    } else {
        return true;
    }
}

QString User::toString(){
//    <user id="[uuid]" ip="[ip]" name="[name]" logTime="[logTime]" info="[info]" status="[status]" iconData="[iconData]" />
    QDomDocument userDoc;
    QDomElement userE = userDoc.createElement("user");
    userE.setAttribute("id", uuid.toString());
    userE.setAttribute("ip", ip.toString());
    userE.setAttribute("name", name);
    userE.setAttribute("logTime", logTime.toString());
    userE.setAttribute("info", info);
    userE.setAttribute("status", (int)status);


    userE.setAttribute("iconData", getCompressDataFromIcon(icon));

    userDoc.appendChild(userE);
    return userDoc.toString(-1);
}

QString User::getCompressDataFromIcon(QIcon icon){
    Image img = (Image)icon.pixmap(72, 72).toImage();
    return img.toBase64Data("PNG", true);
}

QIcon User::getIconFromCompressData(QString data){
    Image img = Image::fromBase64Data(data, true);
    return QIcon(QPixmap::fromImage(img));
}

QUuid User::getUuid(){
    return this->uuid;
}

QHostAddress User::getIp(){
    return this->ip;
}

QString User::getName(){
    return this->name;
}

QIcon User::getIcon(){
    return this->icon;
}

QDateTime User::getLogTime(){
    return this->logTime;
}

QString User::getInfo(){
    return this->info;
}

User::Status User::getStatus(){
    return this->status;
}
QString User::getStatusStr(){
    switch (status) {
    case Online:
        return "Online";
        break;
    case Leaving:
        return "Leaving";
        break;
    case Busy:
        return "Busy";
        break;
    case Hiding:
        return "Hiding";
        break;
    case Offline:
        return "Offline";
        break;
    default:
        break;
    }
}

void User::setUuid(const QUuid &uuid){
    this->uuid = uuid;
}

void User::setIp(const QHostAddress &ip){
    this->ip = ip;
}

void User::setName(const QString &name){
    this->name = name;
}

void User::setIcon(const QIcon &icon){
    this->icon = icon;
}

void User::setLogTime(const QDateTime &logTime){
    this->logTime = logTime;
}

void User::setInfo(const QString &info){
    this->info = info;
}

void User::setStatus(Status status){
    this->status = status;
}
