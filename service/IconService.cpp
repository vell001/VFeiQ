#include "IconService.h"

IconService::IconService(QObject *parent) :
    QObject(parent)
{
}

IconService *IconService::getService(){
    static IconService service;
    return &service;
}

QIcon IconService::getIconByUuid(const QUuid &uuid){
    if(uuid.isNull()) {
        return QIcon(":/images/default_user_icon.png");
    } else {
        return QIcon(":/images/default_user_icon.png"); ////////////
    }
}
