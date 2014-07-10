/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**      'uuid' will auto create when new a object
**********************************************************************/

#ifndef FRIEND_H
#define FRIEND_H

#include <QObject>
#include <QUuid>
#include <QIcon>
#include <QDateTime>
#include <QHostAddress>

class Friend : public QObject
{
    Q_OBJECT
public:
    explicit Friend(QObject *parent = 0);
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

    QUuid getUuid();
    QHostAddress getIp();
    QString getName();
    QIcon getIcon();
    QDateTime getLogTime();
    QString getInfo();
    int getStatus();

    void setUuid(const QUuid &uuid);
    void setIp(const QHostAddress &ip);
    void setName(const QString &name);
    void setIcon(const QIcon &icon);
    void setLogTime(const QDateTime &logTime);
    void setInfo(const QString &info);
    void setStatus(int status);

signals:

public slots:
private:
    QUuid uuid;
    QHostAddress ip;
    QString name;
    QIcon icon;
    QDateTime logTime;
    QString info;
    int status;
};

#endif // FRIEND_H
