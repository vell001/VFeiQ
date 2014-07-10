#include "Friend.h"

Friend::Friend(QObject *parent) :
    QObject(parent)
{
    this->uuid = QUuid::createUuid();
}

QUuid Friend::getUuid(){
    return this->uuid;
}

QHostAddress Friend::getIp(){
    return this->ip;
}

QString Friend::getName(){
    return this->name;
}

QIcon Friend::getIcon(){
    return this->icon;
}

QDateTime Friend::getLogTime(){
    return this->logTime;
}

QString Friend::getInfo(){
    return this->info;
}

int Friend::getStatus(){
    return this->status;
}

void Friend::setUuid(const QUuid &uuid){
    this->uuid = uuid;
}

void Friend::setIp(const QHostAddress &ip){
    this->ip = ip;
}

void Friend::setName(const QString &name){
    this->name = name;
}

void Friend::setIcon(const QIcon &icon){
    this->icon = icon;
}

void Friend::setLogTime(const QDateTime &logTime){
    this->logTime = logTime;
}

void Friend::setInfo(const QString &info){
    this->info = info;
}

void Friend::setStatus(int status){
    this->status = status;
}
