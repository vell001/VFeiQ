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
    void senderError(QString error);
    void bytesWritten ( qint64 bytes );
public slots:
    void connected();
    void disconnected();

private:
    QUuid uuid;
    QHostAddress receiverIp;
    quint16 receiverPort;
    QFile *file;

    QTcpSocket *mSocket;
};

#endif // FILESENDER_H
