#include "FileShareService.h"

FileShareService::FileShareService(quint16 chatport, QObject *parent) :
    UdpService(chatport, parent),
    mSharedFilesMessages(0),
    mFileSender(0),
    sender(UserService::getService()->getMyself())
{
    listen();
}

void FileShareService::listen(){
    connect(this, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(messageReceived(QHostAddress,quint16,ChatMessage)));
}

FileShareService *FileShareService::getService(){
    static FileShareService service(5103);
    return &service;
}

void FileShareService::sendSharedFilesRequest(const QHostAddress &receiverIp){
    ChatMessage message(ChatMessage::Request, sender->getUuid(), "requestSharedFiles");
    UdpService::send(message, receiverIp);
}

void FileShareService::messageReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(message.getMode() == ChatMessage::Request) {
        if(message.getContentType() == ChatMessage::Text) {
            if(message.getContent() == "requestSharedFiles") { // response my shared files
                delete mSharedFilesMessages;
                mSharedFilesMessages = StorageService::getService()->getSharedFilesMessages();
                QString fileMessagesXMLStr = FileMessage::fileMessagesToXMLStr(*mSharedFilesMessages);
                ChatMessage respMessage(ChatMessage::Response, sender->getUuid(), fileMessagesXMLStr, ChatMessage::FilesXML);
                UdpService::send(respMessage, senderIp);
            }
        } else if(message.getContentType() == ChatMessage::FileXML) { // send zhe file
            FileMessage fMsg(message.getContent());
            if(mSharedFilesMessages->contains(fMsg.getUuid())) { // exist zhe file
                delete mFileSender;
                mFileSender = new FileSender(new QFile(fMsg.getFileFullName()),
                                             senderIp,
                                             fMsg.getTransferPort(),
                                             fMsg.getUuid());
                mFileSender->send();
            }
        }
    } else if(message.getMode() == ChatMessage::Response) {
        if(message.getContentType() == ChatMessage::FilesXML) { // got shared files
            emit receivedSharedFiles(senderIp, senderPort, message);
        }
    }
}
