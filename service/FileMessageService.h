#ifndef FILEMESSAGESERVICE_H
#define FILEMESSAGESERVICE_H

#include <QObject>
#include "service/UdpService.h"

class FileMessageService : public UdpService
{
    Q_OBJECT
public:
    static FileMessageService *getService();
signals:
public slots:
protected:
    explicit FileMessageService(QObject *parent = 0);
};

#endif // FILEMESSAGESERVICE_H
