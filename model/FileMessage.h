#ifndef FILEMESSAGE_H
#define FILEMESSAGE_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QFileInfo>
#include <QtXml>
#include <QHash>

class FileMessage : public QObject
{
    Q_OBJECT
public:
    enum Type{
        Dir,
        File
    };

    explicit FileMessage(QObject *parent = 0);
    explicit FileMessage(const QFileInfo &info, quint16 transferPort = 1213, QObject *parent = 0);
    explicit FileMessage(const QString &infoStr, QObject *parent = 0);
    FileMessage &operator=(const FileMessage &fileMessage);
    QString toString();

    static QHash<QUuid, FileMessage *> parseFileMessages(const QString &messagesStr);
    static QString fileMessagesToString(const Hash<QUuid, FileMessage *> &fileMessages);

    QUuid getUuid();
    QString getFileName();
    QString getFileFullName();
    qint64 getSize();
    Type getType();
    quint16 getTransferPort();

    void setUuid(const QUuid &uuid);
    void setFileName(const QString &fileName);
    void setFileFullName(const QString &fileFullName);
    void setSize(qint64 size);
    void setType(Type type);
    void setTransferPort(quint16 transferPort);
signals:
    void parseError(QString errorMessage);
public slots:
private:
    QUuid uuid;
    QString fileName;
    QString fileFullName;
    qint64 size;
    Type type;
    quint16 transferPort;
};

#endif // FILEMESSAGE_H
