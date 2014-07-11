#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mSettingUtil = SettingUtil::getUtil();

    mFriends = new QList<User>;
    mFriends->push_back(*mSettingUtil->getSender());

    mBroadcastService = BroadcastService::getService();
    ChatMessage reqMes(ChatMessage::Request, mSettingUtil->getSender()->getUuid(), QByteArray("request"));
    mBroadcastService->send(reqMes, QHostAddress("255.255.255.255"));

    connect(mBroadcastService, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(received(QHostAddress,quint16,ChatMessage)));

    ui->userImage->setIconSize(QSize(50, 50));

    ui->weatherLabel->setText(QString("<img src=\":/images/weather_1.gif\"/>"));
    ui->searchLabel->setText(QString("<img src=\":/images/search.png\" width='24' height='24'/>"));
    ui->signatureLabel->setText(QString("signatrue"));
    ui->contentsTreeWidget->setIconSize(QSize(40, 40));

    connect(ui->contentsTreeWidget,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openChatForm(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mFriends;
}

void MainWindow::openChatForm(QModelIndex index){
    qDebug() << "openChatForm " << index.row();
    if(ui->contentsTreeWidget->currentItem()->childCount() == 0){
        QUuid userUuid = QUuid(index.data(Qt::UserRole).toString());
        int index = User::findUser(*mFriends, userUuid);
        (new ChatForm((*mFriends)[index]))->show();
    }
}

void MainWindow::received(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(message.getType() == ChatMessage::Response){
        User user(message.getContent());

        if(user.getIcon().isNull()) {
            user.setIcon(QIcon(QString(":/images/user_image.png")));
        }
        if(user.getIp().isNull() || user.getIp() != senderIp) {
            user.setIp(senderIp);
        }
        if(user.getLogTime().isNull() || user.getLogTime().isValid()) {
            user.setLogTime(QDateTime::currentDateTime());
        }
        if(user.getStatus() == User::OffLine) {
            qDebug() << user.getName() << "下线了";
        }
        if(user.getUuid() == mSettingUtil->getSender()->getUuid()) { // 初始化本机
            *mSettingUtil->getSender() = user;
            (*mFriends)[0] = user;
            ui->userImage->setIcon(user.getIcon());
        } else { // 新好友
            mFriends->push_back(user);
        }
        qDebug() << user.toString();
        updateContentsTreeWidget();

    } else if(message.getType() == ChatMessage::Request) {
        ChatMessage myselfMessage(ChatMessage::Response, mSettingUtil->getSender()->getUuid(), mSettingUtil->getSender()->toQByteArray(), this);
        mBroadcastService->send(myselfMessage, senderIp);
    }
}

void MainWindow::updateContentsTreeWidget(){
    ui->contentsTreeWidget->clear();
    for(int i=0; i<1; i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->contentsTreeWidget);
        item->setText(0,QString("defualt"));
        User *user;
        for(int j=0; j<mFriends->count(); j++) {
            user = &(*mFriends)[j];
            QTreeWidgetItem *cItem = new QTreeWidgetItem(item);
            cItem->setText(0,QString("%1\r\n%2").arg(user->getName()).arg(user->getIp().toString()));
            cItem->setData(0, Qt::UserRole, user->getUuid().toString());
            cItem->setIcon(0, user->getIcon());
        }
    }
}

