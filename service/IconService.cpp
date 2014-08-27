#include "IconService.h"

IconService::IconService(QObject *parent) :
    UdpService(5732, parent),
    mStorageService(StorageService::getService()),
    myself(UserService::getService()->getMyself()),
    mFileSender(0),
    mFileReceiver(0),
    receivingUuid(QUuid()),
    customIconFilesDir(QDir(QCoreApplication::applicationDirPath()+"/icons")),
    normalIconFilesDir(QDir(":/userIcons")),
    normalUserIcons(new QHash<QUuid, Icon *>),
    customUserIcons(new QHash<QUuid, Icon *>)
{
    connect(this, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(iconInfoReceived(QHostAddress,quint16,ChatMessage)));
    updateCustomUserIconsFromDir();
    initNormalUserIcons();
}

IconService *IconService::getService(){
    static IconService service;
    return &service;
}

QIcon IconService::getUserIconByUuid(const QUuid &uuid){
    if(uuid.isNull()) { // defualt
        return QIcon(":/images/default_user_icon.png");
    } else if(normalUserIcons->contains(uuid)){
        return (*normalUserIcons)[uuid]->getIcon();
    } else if(customUserIcons->contains(uuid)){ // exist
        return (*customUserIcons)[uuid]->getIcon();
    } else { // not exist
        return QIcon(":/images/default_user_icon.png");
    }
}

bool IconService::containsUserIcon(const QUuid &uuid){
    if(normalUserIcons->contains(uuid) || customUserIcons->contains(uuid)){ // exist
        return true;
    } else {
        return false;
    }
}

void IconService::addIconToGet(const QUuid &uuid, const QHostAddress &address){
    mIconsToGet[uuid] = address;
    if(mFileReceiver == 0) { // no file sending now
        fileReceiveEnd(-1);
    }
}

void IconService::getIconFromAddress(const QUuid &uuid, const QHostAddress &address){
    QFile *saveFile = new QFile(customIconFilesDir.absolutePath()+"/"+uuid.toString());
    mFileReceiver = new FileReceiver(saveFile, address, 3474, uuid);

    connect(mFileReceiver, SIGNAL(receiveEnd(qint64)), this, SLOT(fileReceiveEnd(qint64)));
    connect(mFileReceiver, SIGNAL(receiveError(QString)), this, SIGNAL(fileReceiveError(QString)));

    mFileReceiver->start();

    ChatMessage iconInfoMsg(uuid, ChatMessage::Request, myself->getUuid(), uuid.toString(), ChatMessage::IconUuid);
    UdpService::send(iconInfoMsg, address);
}

void IconService::iconInfoReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(message.getMode() == ChatMessage::Request) {
        if(message.getContentType() == ChatMessage::IconUuid) {
            updateCustomUserIconsFromDir();
            QUuid iconUuid(message.getContent());
            if(customUserIcons->contains(iconUuid)) {
                delete mFileSender;
                Icon *icon = (*customUserIcons)[iconUuid];
                mFileSender = new FileSender(new QFile(icon->getFileInfo().filePath()),
                                             senderIp,
                                             3474,
                                             message.getUuid());
                mFileSender->send();
            } else { // not exist
                ChatMessage msg(ChatMessage::Response, myself->getUuid(), "not exist", ChatMessage::Text);
                UdpService::send(msg, senderIp);
            }
        }
    } else if(message.getMode() == ChatMessage::Response){
        if(message.getContentType() == ChatMessage::Text) {
            emit fileReceiveError(message.getContent());
        }
    }
}

void IconService::fileReceiveEnd(qint64 totalBytes){
    delete mFileReceiver;
    mFileReceiver = 0;

    if(!receivingUuid.isNull())  {
        mIconsToGet.remove(receivingUuid);

        updateCustomUserIconsFromDir();
        emit iconFileReceiveEnd(receivingUuid);
    }

    if(!mIconsToGet.isEmpty()){
        receivingUuid = mIconsToGet.keys()[0];
        QHostAddress address = mIconsToGet[receivingUuid];
        getIconFromAddress(receivingUuid, address);
    }
}

void IconService::updateCustomUserIcons(){
    delete customUserIcons;
    customUserIcons = mStorageService->getUserIcons();
}

void IconService::updateCustomUserIconsFromDir(){
    if(!customIconFilesDir.exists()) {
        customIconFilesDir.mkdir(customIconFilesDir.absolutePath());
    }
    QFileInfoList fileList = customIconFilesDir.entryInfoList();

    foreach (QFileInfo fileInfo, fileList) { // add not registe icon
        QUuid uuid(fileInfo.fileName());
        if(!uuid.isNull()) {
            Icon *icon = new Icon(fileInfo, uuid);
            (*customUserIcons)[icon->getUuid()] = icon;
        }
    }
}

void IconService::initNormalUserIcons(){
    QFileInfoList fileList = normalIconFilesDir.entryInfoList();
    foreach (QFileInfo fileInfo, fileList) {
        QUuid uuid = QUuid(fileInfo.baseName());
        Icon *icon = new Icon(fileInfo, uuid);
        (*normalUserIcons)[icon->getUuid()] = icon;
    }
}
