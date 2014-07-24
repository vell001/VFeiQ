#ifndef FILESHARESERVICE_H
#define FILESHARESERVICE_H

#include <QObject>
#include "service/StorageService.h"
#include "service/UserService.h"
#include "model/FileSender.h"
#include "service/UdpService.h"

class FileShareService : public UdpService
{
    Q_OBJECT
public:
    static FileShareService *getService();
    void sendSharedFilesRequest(const QHostAddress &receiverIp);
signals:
    void receivedSharedFiles(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
public slots:
    void messageReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
private :
    explicit FileShareService(quint16 chatport, QObject *parent = 0);
    void listen();
    QHash<QUuid, FileMessage *> *mSharedFilesMessages;
    User *sender;
    FileSender *mFileSender;
};

#endif // FILESHARESERVICE_H
