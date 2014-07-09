#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QObject>
#include <QUuid>
#include <QHostAddress>

class ChatMessage : public QObject
{
    Q_OBJECT
public:
    explicit ChatMessage(QObject *parent = 0);
    explicit ChatMessage(const QString &content, const QHostAddress &receiver, QObject *parent = 0);

    QString getContent();
    QHostAddress getReceiver();
    QUuid getUuid();
    void setContent(const QString& content);
    void setReceiver(const QHostAddress& receiver);
    void setUuid(const QUuid& uuid);
signals:

public slots:

private:
    QString content;
    QHostAddress receiver;
    QUuid uuid;
};

#endif // CHATMESSAGE_H
