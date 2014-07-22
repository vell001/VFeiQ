#include "StorageService.h"

StorageService::StorageService(QObject *parent) :
    QObject(parent)
{
    initStorages();
}

void StorageService::initStorages(){
    QString basePath = QCoreApplication::applicationDirPath()+"/storages";
    QDir baseDir(basePath);
    if(!baseDir.exists()) {
        baseDir.mkdir(basePath);
    }

    mStorages["mSharedFilesStorage"] = new QFile(basePath + "/mSharedFilesStorage");
}

StorageService::~StorageService(){
}

StorageService *StorageService::getService(){
    static StorageService service;
    return &service;
}

QHash<QUuid, FileMessage *> *StorageService::getSharedFilesMessages(){
    QFile *mSharedFilesStorage = mStorages["mSharedFilesStorage"];
    QHash<QUuid, FileMessage *> *fileMessages = 0;
    if(mSharedFilesStorage->open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(mSharedFilesStorage);

        QString text = in.readAll();
        fileMessages = FileMessage::parseFileMessages(text);

        mSharedFilesStorage->close();
    } else {
        fileMessages = new QHash<QUuid, FileMessage *>;
    }
    return fileMessages;
}

bool StorageService::storeSharedFilesMessages(QHash<QUuid, FileMessage *> *fileMessages){
    QFile *mSharedFilesStorage = mStorages["mSharedFilesStorage"];
    if(mSharedFilesStorage->open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(mSharedFilesStorage);

        QString text = FileMessage::fileMessagesToXMLStr(*fileMessages, 1);
        out << text;

        out.flush();
        mSharedFilesStorage->flush();
        mSharedFilesStorage->close();
        return true;
    } else {
        qDebug() << "can't open file: " << mSharedFilesStorage->fileName();
        return false;
    }
}
