#ifndef ICONSERVICE_H
#define ICONSERVICE_H

#include <QObject>

class IconService : public QObject
{
    Q_OBJECT
public:
    static IconService *getService();
signals:

public slots:
private:
    explicit IconService(QObject *parent = 0);
};

#endif // ICONSERVICE_H
