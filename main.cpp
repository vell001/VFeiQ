#include "MainWindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QTextStream>

#include "ChatForm.h"
#include <QTextCodec>
#include <QHostInfo>
#include <QNetworkInterface>
#include "service/ChatService.h"
#include "model/User.h"
#include <QList>

bool setSkin(QApplication* const app, QString const &skinFile)
{
    QFile file(skinFile);

    if (QFile::exists(skinFile) && file.open(QIODevice::ReadOnly))
    {
        QApplication::setStyle(QStyleFactory::create("Windows"));
        QString strTemp;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            strTemp.append(in.readLine());
        }
        file.close();
        app->setStyleSheet(strTemp);
    }
    else
    {
#ifdef Q_WS_MAC
        qDebug("%s: %s: File does not exist %s... setting mac style...",
               __FILE__, __FUNCTION__, qPrintable(skinFile));
        app->setStyle(new QMacStyle());
        return true;
#else
        qDebug("%s: %s: File does not exist or failed to open %s",
               __FILE__, __FUNCTION__, qPrintable(skinFile));
        return false;
#endif
    }

    return true;
}

quint16 ChatPort = 9514;
QList<User> friends;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

//    User user(QByteArray("{{b36fbd92-de5f-482b-9e5b-2f139a9a09b3},,vell001,,,,0}"));
//    qDebug() << user.getIcon().isNull();
//    qDebug() << user.getInfo().isEmpty();
//    qDebug() << user.getIp().isNull();
//    qDebug() << user.getLogTime().isNull();
//    qDebug() << user.getName().isEmpty();

    MainWindow w;
    w.show();

//    User Receiver(QHostAddress("127.0.0.1"), QString("vell002"));
//    ChatForm c(Receiver);
//    c.show();

//    setSkin(&a ,":/skins/defaultSkin.css");

//    ChatUDPSocket cus;

//    ChatMessage mes("hello", QHostAddress("192.168.107.255"));

//    cus.send(&mes);

//    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
//    foreach(QNetworkInterface interface,list)
//    {
//        qDebug() <<"Device:"<<interface.name();
//        qDebug() << "HardwareAddress:"<<interface.hardwareAddress();


//        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
//     //获取IP地址条目列表，每个条目中包含一个IP地址，一个子网掩码和一个广播地址
//        foreach(QNetworkAddressEntry entry,entryList)
//        {
//            qDebug()<<"IP Address:"<<entry.ip().toString();
//            qDebug()<<"Netmask: "  <<entry.netmask().toString();
//            qDebug()<<"Broadcast:" <<entry.broadcast().toString();
//        }
//        qDebug() << "";
//    }
    return a.exec();
}
