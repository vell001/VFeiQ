#ifndef ICON_H
#define ICON_H

#include <QObject>
#include <QUuid>
#include <QIcon>

class Icon : public QObject
{
    Q_OBJECT
public:
    explicit Icon(QObject *parent = 0);
    explicit Icon(const QString &iconXMLStr, QObject *parent = 0);
    explicit Icon(const QUuid &uuid, const QString &fileFullName, QObject *parent = 0);

    QString toXMLString();

    QUuid getUuid();
    QIcon getIcon();
    QString getFileName();
    QString getFileFullName();

    void setUuid(const QUuid &uuid);
    void setIcon(const QIcon &icon);
    void setFileName(const QString &fileName);
    void setFileFullName(const QString &fileFullName);
signals:

public slots:
private:
    QUuid uuid;
    QIcon icon;
    QString fileName;
    QString fileFullName;
};

#endif // ICON_H
