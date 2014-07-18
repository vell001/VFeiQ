#include "FileMessage.h"

FileMessage::FileMessage(QObject *parent) :
    QObject(parent),
    uuid(QUuid::createUuid())
{
}

FileMessage &FileMessage::operator=(const FileMessage &fileMessage){
    uuid = fileMessage.uuid;
    fileName = fileMessage.fileName;
    fileFullName = fileMessage.fileFullName;
    size = fileMessage.size;
    type = fileMessage.type;
    transferPort = fileMessage.transferPort;
    return *this;
}

FileMessage::FileMessage(const QFileInfo &info, quint16 transferPort, QObject *parent) :
    QObject(parent),
    uuid(QUuid::createUuid()),
    fileName(info.fileName()),
    fileFullName(info.filePath()),
    size(info.size()),
    transferPort(transferPort)
{
    if(info.isDir()){
        type = Dir;
    } else if(info.isFile()){
        type = File;
    }
}

FileMessage::FileMessage(const QString &infoStr, QObject *parent) :
    QObject(parent)
{
    QDomDocument messageDoc;
    QString errorMsg;
    messageDoc.setContent(infoStr, &errorMsg);
    if(!errorMsg.isEmpty()) {
        emit parseError(errorMsg);
    }
    QDomElement msgE = messageDoc.firstChildElement();

    uuid = QUuid(msgE.attribute("id"));
    fileName = msgE.attribute("fileName");
    fileFullName = msgE.attribute("fileFullName");
    size = msgE.attribute("size").toLongLong();
    type = Type(msgE.attribute("type").toInt());
    transferPort = msgE.attribute("transferPort").toUInt();
}

QString FileMessage::toString(){
    QDomDocument messageDoc;
    QDomElement msgE = messageDoc.createElement("file");
    msgE.setAttribute("id", uuid.toString());
    msgE.setAttribute("fileName", fileName);
    msgE.setAttribute("fileFullName", fileFullName);
    msgE.setAttribute("size", size);
    msgE.setAttribute("type", (int)type);
    msgE.setAttribute("transferPort", transferPort);
    messageDoc.appendChild(msgE);
    return messageDoc.toString(-1);
}

QHash<QUuid, FileMessage *> FileMessage::parseFileMessages(const QString &messagesStr){
    QHash<QUuid, FileMessage *> fileMessages;
    QDomDocument messageDoc;
    QString errorMsg;
    messageDoc.setContent(infoStr, &errorMsg);
    if(!errorMsg.isEmpty()) {
        return fileMessages;
    }
    QDomElement msgE = messageDoc.firstChildElement();

    foreach (QDomNode file, msgE.childNodes()) {
        QDomElement fileE = file.toElement();
//        uuid = QUuid(fileE.attribute("id"));
//        fileName = fileE.attribute("fileName");
//        fileFullName = fileE.attribute("fileFullName");
//        size = fileE.attribute("size").toLongLong();
//        type = Type(fileE.attribute("type").toInt());
//        transferPort = fileE.attribute("transferPort").toUInt();
    }
}

QString FileMessage::fileMessagesToString(const Hash<QUuid, FileMessage *> &fileMessages){

}

QUuid FileMessage::getUuid(){
    return uuid;
}

QString FileMessage::getFileName(){
    return fileName;
}

QString FileMessage::getFileFullName(){
    return fileFullName;
}

qint64 FileMessage::getSize(){
    return size;
}

FileMessage::Type FileMessage::getType(){
    return type;
}

quint16 FileMessage::getTransferPort(){
    return transferPort;
}

void FileMessage::setUuid(const QUuid &uuid){
    this->uuid = uuid;
}

void FileMessage::setFileName(const QString &fileName){
    this->fileName = fileName;
}

void FileMessage::setFileFullName(const QString &filePath){
    this->fileFullName = filePath;
}

void FileMessage::setSize(qint64 size){
    this->size = size;
}

void FileMessage::setType(Type type){
    this->type = type;
}

void FileMessage::setTransferPort(quint16 transferPort){
    this->transferPort = transferPort;
}
