#include "ChatRecordService.h"

ChatRecordService::ChatRecordService(QObject *parent) :
    QObject(parent)
{
}
ChatRecordService::~ChatRecordService(){

}

ChatRecordService *ChatRecordService::getService(){
    static ChatRecordService mService;
    return &mService;
}

QList<ChatRecord> *ChatRecordService::getChatRecords(){
    return &mChatRecords;
}

ChatRecord *ChatRecordService::getChatRecordByUuid(QUuid uuid){
    for(int i=0; i<mChatRecords.count(); i++) {
        if(mChatRecords[i].getUuid() == uuid) {
            return &mChatRecords[i];
        }
    }
}

QList<ChatRecord *> ChatRecordService::getChatRecordsByUserUuid(QUuid userUuid){
    QList<ChatRecord *> list;
    for(int i=0; i<mChatRecords.count(); i++) {
        if(mChatRecords[i].getUserUuid() == userUuid) {
            list.append(&mChatRecords[i]);
        }
    }
    return list;
}

QList<ChatRecord *> ChatRecordService::getChatRecordsByUserUuid(QUuid userUuid, ChatRecord::IsRead isRead){
    QList<ChatRecord *> list;
    for(int i=0; i<mChatRecords.count(); i++) {
        if(mChatRecords[i].getUserUuid() == userUuid && mChatRecords[i].getIsRead() == isRead) {
            list.append(&mChatRecords[i]);
        }
    }
    return list;
}
