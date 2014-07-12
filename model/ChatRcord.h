#ifndef CHATRCORD_H
#define CHATRCORD_H

#include <QObject>
#include <QUuid>
#include <QDateTime>
#include "ChatMessage.h"
#include "service/UserService.h"

class ChatRcord : public QObject
{
    Q_OBJECT
public:
    explicit ChatRcord(QObject *parent = 0);
    explicit ChatRcord(ChatMessage message, QObject *parent = 0);
    explicit ChatRcord(const QUuid &uuid, const QUuid &userUuid, const QString &content, QObject *parent = 0);
    enum SendOrReceive {
        Send,
        Receive
    };

    QUuid getUuid();
    QUuid getUserUuid();
    QDateTime getTime();
    QString getContent();
    SendOrReceive getSendOrReceive();

    void setUuid(QUuid uuid);
    void setUserUuid(QUuid userUuid);
    void setTime(QDateTime time);
    void setContent(QString content);
    void setSendOrReceive(SendOrReceive sendOrReceive);

signals:

public slots:

private:
    QUuid uuid;
    QUuid userUuid;
    QDateTime time;
    QString content;
    SendOrReceive sendOrReceive;

    User *myself;
};

#endif // CHATRCORD_H
