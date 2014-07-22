#include "FileMessage.h"

FileMessage::FileMessage(QObject *parent) :
    QObject(parent),
    uuid(QUuid::createUuid()),
    size(0),
    transferPort(0)
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

FileMessage::FileMessage(const QFileInfo &info, const QUuid &uuid, quint16 transferPort, QObject *parent) :
    QObject(parent),
    uuid(uuid),
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

FileMessage::FileMessage(const QString &infoXMLStr, QObject *parent) :
    QObject(parent)
{
    QDomDocument messageDoc;
    QString errorMsg;
    messageDoc.setContent(infoXMLStr, &errorMsg);
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

QHash<QUuid, FileMessage *> *FileMessage::parseFileMessages(const QString &messagesXMLStr){
    QHash<QUuid, FileMessage *> *fileMessages = new QHash<QUuid, FileMessage *>;
    QDomDocument messageDoc;
    QString errorMsg;
    messageDoc.setContent(messagesXMLStr, &errorMsg);
    if(!errorMsg.isEmpty()) {
        return fileMessages;
    }
    QDomElement msgE = messageDoc.firstChildElement();

    FileMessage *fMsg;
    for(int i=0; i<msgE.childNodes().count(); i++) {
        QDomNode file = msgE.childNodes().item(i);
        if(!file.isElement()) continue;
        QDomElement fileE = file.toElement();
        fMsg = new FileMessage();

        fMsg->setUuid(QUuid(fileE.attribute("id")));
        fMsg->setFileName(fileE.attribute("fileName"));
        fMsg->setFileFullName(fileE.attribute("fileFullName"));
        fMsg->setSize(fileE.attribute("size").toLongLong());
        fMsg->setType(Type(fileE.attribute("type").toInt()));
        fMsg->setTransferPort(fileE.attribute("transferPort").toUInt());
        (*fileMessages)[fMsg->getUuid()] = fMsg;
    }
    return fileMessages;
}

QString FileMessage::fileMessagesToXMLStr(const QHash<QUuid, FileMessage *> &fileMessages, int indent){
    QDomDocument messageDoc;
    QDomElement filesE = messageDoc.createElement("files");
    messageDoc.appendChild(filesE);
    foreach (FileMessage *fMsg, fileMessages.values()) {
        QDomElement msgE = messageDoc.createElement("file");

        msgE.setAttribute("id", fMsg->getUuid().toString());
        msgE.setAttribute("fileName", fMsg->getFileName());
        msgE.setAttribute("fileFullName", fMsg->getFileFullName());
        msgE.setAttribute("size", fMsg->getSize());
        msgE.setAttribute("type", (int)fMsg->getType());
        msgE.setAttribute("transferPort", fMsg->getTransferPort());
        filesE.appendChild(msgE);
    }
    return messageDoc.toString(indent);
}

QString FileMessage::fileMessagesToHTMLStr(const QHash<QUuid, FileMessage *> &fileMessages, int indent){
    QDomDocument messageDoc;
    QDomElement filesE = messageDoc.createElement("div");
    filesE.setAttribute("id", "files");
    messageDoc.appendChild(filesE);
    foreach (FileMessage *fMsg, fileMessages.values()) {
        QDomElement msgE = messageDoc.createElement("div");

        msgE.setAttribute("id", fMsg->getUuid().toString());
        QDomText msgT = messageDoc.createTextNode(fMsg->getFileName());
        msgE.appendChild(msgT);
        filesE.appendChild(msgE);
    }
    return messageDoc.toString(indent);
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
