#include "UserService.h"

UserService::UserService(QObject *parent) :
    QObject(parent)
{
    myself.setIcon(QIcon(":/images/user_image.png"));
    myself.setStatus(User::OnLine);
    myself.setName("vell001");
}

UserService *UserService::getService(){
    static UserService Service;
    return &Service;
}

User *UserService::getMyself(){
    return &myself;
}

QHash<QUuid, User> *UserService::getFriends(){
    return &mFriends;
}
