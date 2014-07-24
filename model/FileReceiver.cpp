#include "FileReceiver.h"

FileReceiver::FileReceiver(QObject *parent) :
    QObject(parent)
{
    connect(&tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

FileReceiver::FileReceiver(QFile *file, const QHostAddress &senderIp, quint16 receivePort, const QUuid &uuid, QObject *parent) :
    QObject(parent),
    file(file),
    senderIp(senderIp),
    receivePort(receivePort),
    uuid(uuid)
{
    connect(&tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

void FileReceiver::start(){
    totalBytes = 0;
    bytesReceived = 0;
    uuidSize = 0;
    if(!tcpServer.listen(senderIp,receivePort))
    {
        emit receiveError(tcpServer.errorString());
        qDebug() << tcpServer.errorString();
        tcpServer.close();
        return;
    } else {
        qDebug() << "Receiver start";
    }
}

void FileReceiver::acceptConnection(){
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection,SIGNAL(readyRead()),this,SLOT(updateServerProgress()));
    connect(tcpServerConnection,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(acceptError(QAbstractSocket::SocketError)));
    qDebug() << "Receiver acceptConnection";
}

void FileReceiver::updateServerProgress(){
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_8);

    if(bytesReceived <= sizeof(qint64)*2)
    { //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息

        if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2)&& (uuidSize == 0))
        { //接收数据总大小信息和文件名大小信息
            in >> totalBytes >> uuidSize;
            bytesReceived += sizeof(qint64) * 2;
        }

        if((tcpServerConnection->bytesAvailable() >= uuidSize)
                && (uuidSize != 0))
        {
            QString uuidStr;
            in >> uuidStr;
            if(QUuid(uuidStr) != uuid){
                qDebug() << "receive a unexcepted file";
                tcpServerConnection->close();
                return;
            } else {
                tcpServer.close(); // 接收到对应的文件
            }
            bytesReceived += uuidSize;
            if(!file->open(QFile::WriteOnly))
            {
                qDebug() << "receiver: open file error!";
                emit receiveError("open file error!");
                return;
            }
        }
        else return;
    }

    if(bytesReceived < totalBytes)
    {  //如果接收的数据小于总数据，那么写入文件
        bytesReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        file->write(inBlock);
        inBlock.resize(0);
    }

    emit receiveProgress(totalBytes, bytesReceived);

    qDebug() << "receiveProgress : " << totalBytes << " / " << bytesReceived;

    if(bytesReceived == totalBytes)
    { //接收数据完成时
        tcpServerConnection->close();
        file->close();
        emit receiveEnd(totalBytes);
    }
}

void FileReceiver::acceptError(QAbstractSocket::SocketError error){
    qDebug() << tcpServerConnection->errorString();
    emit receiveError(tcpServerConnection->errorString());
}

FileReceiver::~FileReceiver(){
    delete file;
}

QUuid FileReceiver::getUuid(){
    return uuid;
}

quint16 FileReceiver::getReceivePort(){
    return receivePort;
}

QHostAddress FileReceiver::getSenderIp(){
    return senderIp;
}

QFile *FileReceiver::getFile(){
    return file;
}

void FileReceiver::setUuid(const QUuid &uuid){
    this->uuid = uuid;
}

void FileReceiver::setReceivePort(quint16 receivePort){
    this->receivePort = receivePort;
}

void FileReceiver::setSenderIp(const QHostAddress &senderIp){
    this->senderIp = senderIp;
}

void FileReceiver::setFile(QFile *file){
    delete this->file;
    this->file = file;
}
