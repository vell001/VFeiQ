#include "Icon.h"

Icon::Icon(QObject *parent) :
    QObject(parent)
{
}
Icon::Icon(const QString &iconXMLStr, QObject *parent) :
    QObject(parent)
{

}

Icon::Icon(const QUuid &uuid, const QString &fileFullName, QObject *parent) :
    QObject(parent),
    fileFullName(fileFullName)
{
}

QString Icon::toXMLString(){
}

QUuid Icon::getUuid(){
    return uuid;
}

QIcon Icon::getIcon(){
    return icon;
}

QString Icon::getFileName(){
    return fileName;
}

QString Icon::getFileFullName(){
    return fileFullName;
}

void Icon::setUuid(const QUuid &uuid){
    this->uuid = uuid;
}

void Icon::setIcon(const QIcon &icon){
    this->icon = icon;
}

void Icon::setFileName(const QString &fileName){
    this->fileName = fileName;
}

void Icon::setFileFullName(const QString &fileFullName){
    this->fileFullName = fileFullName;
}
