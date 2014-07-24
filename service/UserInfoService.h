#ifndef USERINFOSERVICE_H
#define USERINFOSERVICE_H

#include <QObject>

#include "UdpService.h"
#include "UserService.h"

class UserInfoService : public UdpService
{
    Q_OBJECT
public:
    static UserInfoService *getService();
signals:
public slots:
    void sendMyselfInfo(QHostAddress ipAddress = QHostAddress("255.255.255.255"));
protected:
private:
    explicit UserInfoService(QObject *parent = 0);
    UserService *mUserService;
    User *myself;
};

#endif // USERINFOSERVICE_H
