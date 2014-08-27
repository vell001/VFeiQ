#ifndef ICONSERVICE_H
#define ICONSERVICE_H

#include <QObject>
#include <QUuid>
#include <QIcon>
#include <QHostAddress>
#include <model/FileSender.h>
#include <model/FileReceiver.h>
#include "StorageService.h"
#include "UdpService.h"
#include "UserService.h"

class IconService : public UdpService
{
    Q_OBJECT
public:
    static IconService *getService();
    QIcon getUserIconByUuid(const QUuid &uuid = QUuid());
    bool containsUserIcon(const QUuid &uuid);
    void addIconToGet(const QUuid &uuid, const QHostAddress &address);
    void updateCustomUserIconsFromDir();

    const QHash<QUuid, Icon *> *getNormalUserIcons() {
        return normalUserIcons;
    }
    const QHash<QUuid, Icon *> *getCustomUserIcons(){
        return customUserIcons;
    }

signals:
    void fileReceiveError(QString errorString);
    void iconFileReceiveEnd(QUuid);
public slots:
    void iconInfoReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message);
    void fileReceiveEnd(qint64 totalBytes);
private:
    explicit IconService(QObject *parent = 0);
    User *myself;
    StorageService *mStorageService;
    QHash<QUuid, Icon *> *customUserIcons;
    QHash<QUuid, Icon *> *normalUserIcons;
    QDir customIconFilesDir;
    QDir normalIconFilesDir;
    void updateCustomUserIcons();
    void initNormalUserIcons();

    FileSender *mFileSender;
    FileReceiver *mFileReceiver;
    QUuid receivingUuid;

    QHash<QUuid, QHostAddress> mIconsToGet;
    void getIconFromAddress(const QUuid &uuid, const QHostAddress &address);
};

#endif // ICONSERVICE_H
