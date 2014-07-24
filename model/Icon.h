#ifndef ICON_H
#define ICON_H

#include <QObject>
#include <QUuid>
#include <QIcon>
#include <QFileInfo>
#include <QtXml>
#include <QHostAddress>

class Icon : public QObject
{
    Q_OBJECT
public:
    explicit Icon(QObject *parent = 0);
    explicit Icon(const QString &iconXMLStr, const QHostAddress &senderIp, QObject *parent = 0);
    explicit Icon(const QFileInfo &fileInfo, const QUuid &uuid = QUuid::createUuid(), QObject *parent = 0);

    QString toXMLString(int indent = -1);
    static QHash<QUuid, Icon *> *parseIconsFromXMLString(const QString &XMLStr);
    static QString IconsToXMLString(const QHash<QUuid, Icon *> &icons, int indent = -1);

    QUuid getUuid();
    QIcon getIcon();
    QFileInfo getFileInfo();

    void setUuid(const QUuid &uuid);
    void setIcon(const QIcon &icon);
    void setFileInfo(const QFileInfo &fileInfo);
signals:
    void parseError(QString errorMessage);
public slots:
private:
    QUuid uuid;
    QIcon icon;
    QFileInfo fileInfo;
};

#endif // ICON_H
