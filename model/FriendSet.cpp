#include "FriendSet.h"

FriendSet::FriendSet(QObject *parent) :
    QObject(parent)
{
}

FriendSet::FriendSet(const QString &setName, QObject *parent) :
    QObject(parent)
{
    setUuid(QUuid::createUuid());
    setSetName(setName);
    setCreateTime(QDateTime::currentDateTime());
}

QUuid FriendSet::getUuid(){
    return this->uuid;
}

QString FriendSet::getSetName(){
    return this->setName;
}

QString FriendSet::getSetInfo(){
    return this->setInfo;
}

QDateTime FriendSet::getCreateTime(){
    return this->createTime;
}

void FriendSet::setUuid(const QUuid &uuid){
    this->uuid = uuid;
}

void FriendSet::setSetName(const QString &setName){
    this->setName = setName;
}

void FriendSet::setSetInfo(const QString &setInfo){
    this->setInfo = setInfo;
}

void FriendSet::setCreateTime(const QDateTime &createTime){
    this->createTime = createTime;
}
