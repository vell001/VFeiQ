#include "IconService.h"

IconService::IconService(QObject *parent) :
    QObject(parent)
{
}

IconService *IconService::getService(){
    static IconService service;
    return &service;
}

QIcon IconService::getIconByUuid(const QUuid &uuid = QUuid()){
    if(uuid.isNull()) {
        return QIcon(":/images/default_user_icon.png");
    } else {

    }
}
