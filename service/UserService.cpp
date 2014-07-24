#include "UserService.h"

UserService::UserService(QObject *parent) :
    QObject(parent),
    mStorageService(StorageService::getService())
{
    myself = mStorageService->getMyself();
    myself->setStatus(User::OnLine);
}

UserService *UserService::getService(){
    static UserService Service;
    return &Service;
}

User *UserService::getMyself(){
    return myself;
}

void UserService::saveMyself(){
    mStorageService->storeMyself(myself);
}

QHash<QUuid, User> *UserService::getFriends(){
    return &mFriends;
}

QHash<QString, User> *UserService::getRecentFriends(){
    return &mRecentFriends;
}

void UserService::insertRecentFriend(const User &user){
    mRecentFriends[QDateTime::currentDateTime().toString()] = user;
}

UserService::~UserService(){
    mStorageService->storeMyself(myself);
}
