#ifndef SETTINGUTIL_H
#define SETTINGUTIL_H

#include <QObject>
#include "model/User.h"

class SettingUtil : public QObject
{
    Q_OBJECT
public:
    static SettingUtil *getUtil();

    User *getSender();
signals:

public slots:
private:
    explicit SettingUtil(QObject *parent = 0);
    User sender;
};

#endif // SETTINGUTIL_H
