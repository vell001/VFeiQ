#include "SettingUtil.h"

SettingUtil::SettingUtil(QObject *parent) :
    QObject(parent)
{
    sender.setName("vell001");
}

SettingUtil *SettingUtil::getUtil(){
    static SettingUtil util;
    return &util;
}

User *SettingUtil::getSender(){
    return &sender;
}
