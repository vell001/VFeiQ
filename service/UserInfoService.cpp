#include "UserInfoService.h"

UserInfoService::UserInfoService(QObject *parent) :
    UdpService(9703, parent),
    mUserService(UserService::getService()),
    myself(UserService::getService()->getMyself())
{
}

UserInfoService *UserInfoService::getService(){
    static UserInfoService mService;
    return &mService;
}

void UserInfoService::sendMyselfInfo(QHostAddress ipAddress){
    ChatMessage userInfoMes(ChatMessage::Request, myself->getUuid(), myself->toString(), ChatMessage::UserXML);
    UdpService::send(userInfoMes, ipAddress);
}
