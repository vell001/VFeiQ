#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QObject>
#include "model/User.h"
#include <QSettings>
#include <QDateTime>

class UserService : public QObject
{
    Q_OBJECT
public:
    explicit UserService(QObject *parent = 0);
    ~UserService();
    static UserService *getService();
    QHash<QUuid, User> *getFriends();
    QHash<QString, User> *getRecentFriends();
    void insertRecentFriend(const User &user);
    User *getMyself();
signals:

public slots:
private:
    User myself;
    QHash<QUuid, User> mFriends;
    QHash<QString, User>  mRecentFriends;
    void saveSettings();
    void loadSettings();
};

#endif // USERSERVICE_H
