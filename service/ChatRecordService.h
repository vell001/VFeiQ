#ifndef CHATRECORDSERVICE_H
#define CHATRECORDSERVICE_H

#include <QObject>
#include "model/ChatRecord.h"

class ChatRecordService : public QObject
{
    Q_OBJECT
public:
    explicit ChatRecordService(QObject *parent = 0);
    ~ChatRecordService();
    static ChatRecordService *getService();
    QList<ChatRecord> *getChatRecords();
    ChatRecord *getChatRecordByUuid(QUuid uuid);
    QList<ChatRecord *> getChatRecordsByUserUuid(QUuid userUuid);
    QList<ChatRecord *> getChatRecordsByUserUuid(QUuid userUuid, ChatRecord::IsRead isRead);
signals:

public slots:
private:
    QList<ChatRecord> mChatRecords;
};

#endif // CHATRECORDSERVICE_H
