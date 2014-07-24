#include "FileMessageService.h"

FileMessageService::FileMessageService(QObject *parent) :
    UdpService(1212, parent)
{
}
FileMessageService *FileMessageService::getService(){
    static FileMessageService mService;
    return &mService;
}
