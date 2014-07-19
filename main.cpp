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
#include "model/FileSender.h"
#include "model/FileReceiver.h"

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

//quint16 ChatPort = 9514;
//QList<User> friends;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

//    QFile *sendfile;
//    QFile *receivefile;
//    FileSender *sender;
//    FileSender *sender2;
//    FileReceiver *receiver;

//    sendfile = new QFile("D:/Music/周杰伦-简单爱.mp3");
//    receivefile = new QFile("D:/周杰伦-简单爱.mp3");
//    sender = new FileSender(sendfile, QHostAddress::LocalHost, 1234, QUuid("{82f1339f-2197-4ce2-8c42-477069c65de7}"));
//    sender2 = new FileSender(sendfile, QHostAddress::LocalHost, 1234, QUuid("{82f1339f-2197-4ce2-8c42-477069c65de8}"));
//    receiver = new FileReceiver(receivefile, QHostAddress::LocalHost, 1234, QUuid("{82f1339f-2197-4ce2-8c42-477069c65de8}"));
//    receiver->start();


//    sender2->send();
//    sender->send();

    QIcon icon(":/images/default_user_icon.png");
    qDebug() << icon.name();
    MainWindow *w = MainWindow::getMainWindow();
    w->show();

//    setSkin(&a ,":/skins/defaultSkin.css");

    return a.exec();
}
