/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**
**********************************************************************/
#ifndef FRIENDSET_H
#define FRIENDSET_H

#include <QObject>
#include <QDateTime>
#include <QUuid>

class FriendSet : public QObject
{
    Q_OBJECT
public:
    explicit FriendSet(QObject *parent = 0);
    explicit FriendSet(const QString &setName, QObject *parent = 0);

    QUuid getUuid();
    QString getSetName();
    QString getSetInfo();
    QDateTime getCreateTime();

    void setUuid(const QUuid &uuid);
    void setSetName(const QString &setName);
    void setSetInfo(const QString &setInfo);
    void setCreateTime(const QDateTime &createTime);
signals:

public slots:
private:
    QUuid uuid;
    QString setName;
    QString setInfo;
    QDateTime createTime;
};

#endif // FRIENDSET_H
