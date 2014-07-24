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
#include <QString>
#include <QStringList>
#include <QtXml>
#include <QDateTime>

class ChatMessage : public QObject
{
    Q_OBJECT
public:
    enum Mode {
        Request,
        Response
    };

    enum ContentType {
        Text,
        FileXML,
        FilesXML,
        UserXML,
        IconUuid
    };

    ChatMessage(Mode mode, const QUuid &senderUuid, const QString &content,
                ContentType contentType = Text, const QDateTime &createTime=QDateTime::currentDateTime(), QObject *parent = 0);
    ChatMessage(const QUuid &uuid, Mode mode, const QUuid &senderUuid, const QString &content,
                ContentType contentType = Text, const QDateTime &createTime=QDateTime::currentDateTime(), QObject *parent = 0);
    ChatMessage(const QString &messageStr, QObject *parent = 0);
    ChatMessage(const ChatMessage &cm);

    QString toString();

    QUuid getUuid();
    QUuid getSenderUuid();
    Mode getMode();
    QString getContent();
    ContentType getContentType();
    QDateTime getCreateTime();

    void setUuid(const QUuid &uuid);
    void setSenderUuid(const QUuid &senderUuid);
    void setMode(Mode mode);
    void setContent(const QString &content);
    void setContentType(ContentType contentType);
    void setCreateTime(const QDateTime &createTime);

    ChatMessage &operator=(const ChatMessage &cm);
signals:
    void parseError(QString errorMessage);
public slots:

private:
    QUuid uuid;
    QUuid senderUuid;
    Mode mode;
    QString content;
    ContentType contentType;
    QDateTime createTime;
};

#endif // CHATMESSAGE_H
