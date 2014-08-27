#ifndef STORAGESERVICE_H
#define STORAGESERVICE_H

#include <QObject>
#include "model/FileMessage.h"
#include "model/User.h"
#include "model/Icon.h"
#include <QDir>
#include <QHash>

class StorageService : public QObject
{
    Q_OBJECT
public:
    explicit StorageService(QObject *parent = 0);
    ~StorageService();
    static StorageService *getService();

    QHash<QUuid, FileMessage *> *getSharedFilesMessages();
    bool storeSharedFilesMessages(QHash<QUuid, FileMessage *> *);

    User *getMyself();
    bool storeMyself(User *myself);

    QHash<QUuid, Icon *> *getUserIcons();
    bool storeUserIcons(QHash<QUuid, Icon *> *userIcons);
signals:

public slots:
private:
    QHash<QString, QFile *> mStorages;

    void initStorages();
};

#endif // STORAGESERVICE_H
