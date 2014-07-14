#include "UserService.h"

UserService::UserService(QObject *parent) :
    QObject(parent)
{
    loadSettings();
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

void UserService::saveSettings(){
    QSettings setting("vell001", "VFeiQ");
    setting.beginGroup("userService");
    setting.setValue("myself", QVariant::fromValue(myself.toString()));
    setting.endGroup();
}

void UserService::loadSettings(){
    QSettings setting("vell001", "VFeiQ");
    setting.beginGroup("userService");
    QVariant myselfV = setting.value("myself");
    if(myselfV.isNull()){
        myself.setName("vell001");
    } else {
        myself = User(myselfV.toString());
    }
    myself.setStatus(User::OnLine);
    setting.endGroup();
}

UserService::~UserService(){
    saveSettings();
}
