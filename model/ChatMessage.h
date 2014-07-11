/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**
**********************************************************************/
#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QObject>
#include <QUuid>
#include <QHostAddress>
#include <QByteArray>

class ChatMessage : public QObject
{
    Q_OBJECT
public:
    enum Type {
        Request,
        Response
    };
    ChatMessage(Type type, const QUuid &senderUuid, const QByteArray &content, QObject *parent = 0);
    ChatMessage(const QUuid &uuid, Type type, const QUuid &senderUuid, const QByteArray &content, QObject *parent = 0);
    ChatMessage(const QByteArray &messageStr, QObject *parent = 0);
    ChatMessage(const ChatMessage &cm);

    QString toString();

    QUuid getUuid();
    QUuid getSenderUuid();
    Type getType();
    QByteArray getContent();

    void setUuid(const QUuid &uuid);
    void setSenderUuid(const QUuid &senderUuid);
    void setType(Type type);
    void setContent(const QByteArray &content);

    ChatMessage &operator=(const ChatMessage &cm);
signals:
    void parseError(QString errorMessage);
public slots:

private:
    QUuid uuid;
    QUuid senderUuid;
    Type type;
    QByteArray content;
};

#endif // CHATMESSAGE_H
