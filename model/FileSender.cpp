#include "FileSender.h"

FileSender::FileSender(QObject *parent) :
    QObject(parent)
{
    loadSize = 4*1024;
    totalBytes = 0;
    writtenBytes = 0;
    bytesToWrite = 0;
    mSocket = new QTcpSocket(this);
    connect(mSocket, SIGNAL(connected()), this, SLOT(start()));
    connect(mSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(updateBytesWritten(qint64)));
}

FileSender::FileSender(QFile *file, const QHostAddress &receiverIp, quint16 receiverPort, const QUuid &uuid, QObject *parent) :
    QObject(parent),
    file(file),
    receiverIp(receiverIp),
    receiverPort(receiverPort),
    uuid(uuid)
{
    loadSize = 4*1024;
    totalBytes = 0;
    writtenBytes = 0;
    bytesToWrite = 0;
    mSocket = new QTcpSocket(this);
    connect(mSocket, SIGNAL(connected()), this, SLOT(start()));
    connect(mSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(updateBytesWritten(qint64)));
}

FileSender::~FileSender(){
    delete file;
    delete mSocket;
}

void FileSender::send(){
    mSocket->connectToHost(receiverIp, receiverPort);
    if(!mSocket->waitForConnected(10000)) {
        qDebug() << "cannot connected to " + receiverIp.toString() + " : " + receiverPort;
        emit sendError("cannot connected to " + receiverIp.toString() + " : " + receiverPort);
    } else{
        qDebug() << "send";
    }
}

void FileSender::start(){
    qDebug() << "start send";
    if(!file->open(QFile::ReadOnly)){
        qDebug() << "file cannot open: " << file->fileName();
        emit sendError("file cannot open: " + file->fileName());
        return;
    }
    totalBytes = file->size();

    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_8);
    sendOut << qint64(0) << qint64(0) << uuid.toString();
    qDebug() << "senduuid: " << uuid.toString();
    //依次写入总大小信息空间，文件名大小信息空间，文件名---仅是文件名不含路径，没必要含路径
    totalBytes += outBlock.size();
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
    //totalBytes是文件总大小，即两个quint64的大小+文件名+文件实际内容的大小
    //qint64((outBlock.size() - sizeof(qint64)*2))得到的是文件名大小
    qDebug() << "outBlock: " << outBlock;
    bytesToWrite = totalBytes - mSocket->write(outBlock);
    //发送完头数据后剩余数据的大小，即文件实际内容的大小
    outBlock.resize(0);
}

void FileSender::updateBytesWritten(qint64 numBytes){
    writtenBytes += (int)numBytes;
    //已经发送数据的大小
    if(bytesToWrite > 0) //如果已经发送了数据
    {
        outBlock = file->read(qMin(bytesToWrite,loadSize));
      //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
      //就发送剩余数据的大小
        bytesToWrite -= (int)mSocket->write(outBlock);
       //发送完一次数据后还剩余数据的大小
        outBlock.resize(0);
       //清空发送缓冲区
    }
    else
    {
        file->close(); //如果没有发送任何数据，则关闭文件
    }
    emit sendProgress(totalBytes, writtenBytes);
    qDebug() << "sendProgress: " << totalBytes << " / " << writtenBytes;
    //更新进度条
    if(writtenBytes == totalBytes) //发送完毕
    {
        emit sendEnd(totalBytes);
        qDebug() << "sendEnd: " << totalBytes;
        file->close();
        mSocket->close();
    }
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
    delete this->file;
    this->file = file;
}
