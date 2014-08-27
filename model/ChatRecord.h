#ifndef CHATRCORD_H
#define CHATRCORD_H

#include <QObject>
#include <QUuid>
#include <QDateTime>
#include "ChatMessage.h"
#include "service/UserService.h"

class ChatRecord : public QObject
{
    Q_OBJECT
public:
    explicit ChatRecord(QObject *parent = 0);
    explicit ChatRecord(ChatMessage message, QObject *parent = 0);
    explicit ChatRecord(const QUuid &uuid, const QUuid &userUuid, const QString &content, QObject *parent = 0);
    ChatRecord(const ChatRecord &cr);
    ChatRecord &operator=(const ChatRecord &cr);
    bool operator<(const ChatRecord &cr);

    enum SendOrReceive {
        Send,
        Receive
    };
    enum IsRead {
        Readed,
        NotRead
    };

    QUuid getUuid();
    QUuid getUserUuid();
    QDateTime getTime();
    QString getContent();
    SendOrReceive getSendOrReceive();
    IsRead getIsRead();

    void setUuid(QUuid uuid);
    void setUserUuid(QUuid userUuid);
    void setTime(QDateTime time);
    void setContent(QString content);
    void setSendOrReceive(SendOrReceive sendOrReceive);
    void setIsRead(IsRead isRead);

signals:

public slots:

private:
    QUuid uuid;
    QUuid userUuid;
    QDateTime time;
    QString content;
    SendOrReceive sendOrReceive;
    IsRead isRead;

    User *myself;
    void initSendOrReceive();
};

#endif // CHATRCORD_H
