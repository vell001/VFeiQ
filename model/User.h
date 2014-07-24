/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**      'uuid' will auto create when new a object
**********************************************************************/

#ifndef USER_H
#define USER_H

#include <QObject>
#include <QUuid>
#include <QUuid>
#include <QDateTime>
#include <QHostAddress>
#include <QtXml>

class User : public QObject
{
    Q_OBJECT
public:
    User(QObject *parent = 0);
    User(const QHostAddress &ip, const QString &name, QObject *parent = 0);
    User(const QString &userXMLStr, QObject *parent = 0);
    User(const User& user);
    enum Status {
        OffLine,
        OnLine,
        Leaving,
        Busy,
        Hiding
    };
    /*
     * 0: 离线
     * 1: 上线
     * 2: 离开
     * 3: 忙碌
     * 4: 隐身
     */

    QString toString();

    QUuid getUuid();
    QHostAddress getIp();
    QString getName();
    QUuid getIconUuid();
    QDateTime getLogTime();
    QString getInfo();
    Status getStatus();

    void setUuid(const QUuid &uuid);
    void setIp(const QHostAddress &ip);
    void setName(const QString &name);
    void setIconUuid(const QUuid &iconUuid);
    void setLogTime(const QDateTime &logTime);
    void setInfo(const QString &info);
    void setStatus(Status status);

    User &operator=(const User &user);
    bool operator== ( const User & other ) const;
    bool operator!= ( const User & other ) const;
signals:
    void parseError(QString errorMessage);
public slots:
private:
    QUuid uuid;
    QHostAddress ip;
    QString name;
    QUuid iconUuid;
    QDateTime logTime;
    QString info;
    Status status;
};

#endif // FRIEND_H
