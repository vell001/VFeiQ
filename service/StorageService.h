#ifndef STORAGESERVICE_H
#define STORAGESERVICE_H

#include <QObject>
#include "model/FileMessage.h"
#include <QDir>
#include <QHash>

class StorageService : public QObject
{
    Q_OBJECT
public:
    explicit StorageService(QObject *parent = 0);
    ~StorageService();
    static StorageService *getService();

    QHash<QUuid, FileMessage *> *getFileMessages();
    bool storeFileMessages(QHash<QUuid, FileMessage *> *);
signals:

public slots:
private:
    QHash<QString, QFile *> mStorages;

    void initStorages();
};

#endif // STORAGESERVICE_H
