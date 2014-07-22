#ifndef FILESHARESERVICE_H
#define FILESHARESERVICE_H

#include <QObject>
#include <service/BroadcastService.h>
#include <service/StorageService.h>
#include <service/UserService.h>
#include <model/FileSender.h>

class FileShareService : public QObject
{
    Q_OBJECT
public:
    explicit FileShareService(QObject *parent = 0);
    static FileShareService *getService();
    void send(ChatMessage &message, const QHostAddress &receiverIp);
    void sendSharedFilesRequest(const QHostAddress &receiverIp);
signals:
    void receivedSharedFiles(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
public slots:
    void received(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
private :
    BroadcastService *mBroadcastService;
    void listen();
    QHash<QUuid, FileMessage *> *mSharedFilesMessages;
    User *sender;
    FileSender *mFileSender;
};

#endif // FILESHARESERVICE_H
