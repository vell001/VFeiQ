#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QUuid>
#include <QAbstractSocket>
#include <QTimer>

class FileReceiver : public QObject
{
    Q_OBJECT
public:
    explicit FileReceiver(QObject *parent = 0);
    explicit FileReceiver(QFile *file, const QHostAddress &senderIp, quint16 receivePort, const QUuid &uuid = QUuid::createUuid(), QObject *parent = 0);
    ~FileReceiver();

    QUuid getUuid();
    quint16 getReceivePort();
    QHostAddress getSenderIp();
    QFile *getFile();

    void setUuid(const QUuid &uuid);
    void setReceivePort(quint16 receivePort);
    void setSenderIp(const QHostAddress &senderIp);
    void setFile(QFile *file);

signals:
    void receiveError(QString error);
    void receiveProgress(qint64 maximum, qint64 value);
    void receiveEnd(qint64 totalBytes);
public slots:
    void start();   //开始监听
    void acceptConnection();  //建立连接
    void acceptError(QAbstractSocket::SocketError);
    void updateServerProgress();  //更新进度条，接收数据
private:
    QUuid uuid;
    quint16 receivePort;
    QHostAddress senderIp;
    QFile *file;   //本地文件

    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 uuidSize;  //文件名的大小信息
    QByteArray inBlock;   //数据缓冲区
};

#endif // FILERECEIVER_H
