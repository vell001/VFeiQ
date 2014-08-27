#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QObject>
#include "model/User.h"
#include <QDateTime>
#include "service/StorageService.h"

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
    void saveMyself();
signals:
    void myselfInfoChanged(User *myself);
public slots:
private:
    StorageService *mStorageService;
    User *myself;
    QHash<QUuid, User> mFriends;
    QHash<QString, User>  mRecentFriends;
};

#endif // USERSERVICE_H
