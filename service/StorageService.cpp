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
    mStorages["SharedFilesStorage"] = new QFile(basePath + "/sharedFilesStorage");
    mStorages["MyselfInfoStorage"] = new QFile(basePath + "/myselfInfoStorage");
}

StorageService::~StorageService(){
}

StorageService *StorageService::getService(){
    static StorageService service;
    return &service;
}

QHash<QUuid, FileMessage *> *StorageService::getSharedFilesMessages(){
    QFile *mSharedFilesStorage = mStorages["SharedFilesStorage"];
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
    QFile *mSharedFilesStorage = mStorages["SharedFilesStorage"];
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

User *StorageService::getMyself(){
    QFile *mMyselfInfoStorage = mStorages["MyselfInfoStorage"];
    User *myself = 0;
    if(mMyselfInfoStorage->open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(mMyselfInfoStorage);

        QString text = in.readAll();
        myself = new User(text);

        mMyselfInfoStorage->close();
    } else {
        myself = new User;
        myself->setName("vell001");
        myself->setStatus(User::OnLine);
        storeMyself(myself);
    }
    return myself;
}

bool StorageService::storeMyself(User *myself){
    QFile *mMyselfInfoStorage = mStorages["MyselfInfoStorage"];
    if(mMyselfInfoStorage->open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(mMyselfInfoStorage);

        QString text = myself->toString();
        out << text;

        out.flush();
        mMyselfInfoStorage->flush();
        mMyselfInfoStorage->close();
        return true;
    } else {
        qDebug() << "can't open file: " << mMyselfInfoStorage->fileName();
        return false;
    }
}

QHash<QUuid, VIcon *> *StorageService::getUserIcons(){

}

bool StorageService::storeUserIcons(QHash<QUuid, VIcon *> *userIcons){

}
