#include "FileSender.h"

FileSender::FileSender(QFile *file, const QHostAddress &receiverIp, quint16 receiverPort, const QUuid &uuid, QObject *parent) :
    QObject(parent),
    file(file),
    receiverIp(receiverIp),
    receiverPort(receiverPort),
    uuid(uuid),
    mSocket(new QTcpSocket)
{
}
FileSender::~FileSender(){
    delete mSocket;
}

void FileSender::send(){
    mSocket->connectToHost(receiverIp, receiverPort);
    if(!mSocket->waitForConnected(5000)) {
        emit senderError("cannot connected to " + receiverIp.toString() + " : " + receiverPort);
    }
    connect(mSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(mSocket, SIGNAL(bytesWritten(qint64)), this, SIGNAL(bytesWritten(qint64)));
}
void FileSender::connected(){
    if(!file->open(QFile::ReadOnly)){
        emit senderError("file cannot open");
        return;
    }
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_8);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    sendOut << qint64(0) << qint64(0) << currentFileName;
}

void FileSender::disconnected(){

}

QUuid FileSender::getUuid(){
    return uuid;
}

QHostAddress FileSender::getReceiverIp(){
    return receiverIp;
}
quint16 FileSender::getReceiverPort(){
    return receiverPort;
}

QFile *FileSender::getFile(){
    return file;
}

void FileSender::setUuid(const QUuid &uuid){
    this->uuid = uuid;
}

void FileSender::setReceiverIp(const QHostAddress &receiverIp){
    this->receiverIp = receiverIp;
}

void FileSender::setReceiverPort(quint16 receiverPort){
    this->receiverPort = receiverPort;
}

void FileSender::setFile(QFile *file){
    this->file = file;
}
