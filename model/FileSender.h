#ifndef FILESENDER_H
#define FILESENDER_H

#include <QObject>
#include <QFile>
#include <QHostAddress>
#include <QUuid>
#include <QTcpSocket>

class FileSender : public QObject
{
    Q_OBJECT
public:
    explicit FileSender(QObject *parent = 0);
    explicit FileSender(QFile *file, const QHostAddress &receiverIp, quint16 receiverPort, const QUuid &uuid = QUuid::createUuid(), QObject *parent = 0);
    ~FileSender();
    void send();

    QUuid getUuid();
    QHostAddress getReceiverIp();
    quint16 getReceiverPort();
    QFile *getFile();

    void setUuid(const QUuid &uuid);
    void setReceiverIp(const QHostAddress &receiverIp);
    void setReceiverPort(quint16 receiverPort);
    void setFile(QFile *file);
signals:
    void sendError(QString error);
    void sendProgress(qint64 maximum, qint64 value);
    void sendEnd(qint64 totalBytes);
private slots:
    void start();
    void updateBytesWritten(qint64);

private:
    QUuid uuid;
    QHostAddress receiverIp;
    quint16 receiverPort;
    QFile *file;

    qint64 writtenBytes;
    qint64 bytesToWrite;
    qint64 totalBytes;
    qint64 loadSize;
    QByteArray outBlock;

    QTcpSocket *mSocket;
};

#endif // FILESENDER_H
