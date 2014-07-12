#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myself = UserService::getService()->getMyself();
    mFriends = UserService::getService()->getFriends();
    mChatForms = new QHash<QUuid, ChatForm *>;
    mChatRcords = new QList<ChatRcord>;

    (*mFriends)[myself->getUuid()] = *(myself);

    /* chat service */
    mChatService = ChatService::getService();
    connect(mChatService, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)), this, SLOT(chatReceiveSuccess(QHostAddress,quint16,ChatMessage)));

    /* broadcast service*/
    mBroadcastService = BroadcastService::getService();
    ChatMessage reqMes(ChatMessage::Request, myself->getUuid(), myself->toString(), this);
    mBroadcastService->send(reqMes, QHostAddress("255.255.255.255"));
    connect(mBroadcastService, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(broadcastReceived(QHostAddress,quint16,ChatMessage)));

    /* view */
    ui->userImage->setIconSize(QSize(50, 50));
    ui->userImage->setIcon(myself->getIcon());
    ui->weatherLabel->setText(QString("<img src=\":/images/weather_1.gif\"/>"));
    ui->searchLabel->setText(QString("<img src=\":/images/search.png\" width='24' height='24'/>"));
    ui->signatureLabel->setText(myself->getInfo());
    ui->contentsTreeWidget->setIconSize(QSize(40, 40));
    connect(ui->contentsTreeWidget,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openChatForm(QModelIndex)));

    /* system tray */
    QIcon wIcon = QIcon(":/images/logo.png");
    setWindowIcon(wIcon);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(wIcon);
    trayIcon->setToolTip("VFeiQ");
    createActions();
    createTrayIcon();
    trayIcon->show();
    setWindowTitle("VFeiQ");
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activated(QSystemTrayIcon::ActivationReason)));
}

MainWindow *MainWindow::getMainWindow(){
    static MainWindow w;
    return &w;
}

MainWindow::~MainWindow()
{
    myself->setStatus(User::OffLine);
    ChatMessage myselfMessage(ChatMessage::Response, myself->getUuid(), myself->toString(), this);
    mBroadcastService->send(myselfMessage, QHostAddress("255.255.255.255"));
    delete ui;
    delete mChatForms;
    delete mChatRcords;
}

void MainWindow::openChatForm(QModelIndex index){
    if(ui->contentsTreeWidget->currentItem()->childCount() == 0){
        QUuid userUuid = QUuid(index.data(Qt::UserRole).toString());
        ChatForm *chatForm = new ChatForm(&((*mFriends)[userUuid]));
        (*mChatForms)[userUuid] = chatForm;
        chatForm->show();
    }
}

void MainWindow::broadcastReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
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

    (*mFriends)[message.getSenderUuid()] = user;

    qDebug() << "received" << user.toString();
    updateContentsTreeWidget();

    if(message.getType() == ChatMessage::Request) {
        ChatMessage myselfMessage(ChatMessage::Response, myself->getUuid(), myself->toString(), this);
        mBroadcastService->send(myselfMessage, senderIp);
    }
}

void MainWindow::updateContentsTreeWidget(){
    ui->contentsTreeWidget->clear();
    for(int i=0; i<1; i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->contentsTreeWidget);
        item->setText(0,QString("defualt"));
        User *user;
        QHash<QUuid, User>::iterator it;
        for(it = mFriends->begin(); it != mFriends->end(); it++) {
            user = &it.value();
            QTreeWidgetItem *cItem = new QTreeWidgetItem(item);
            cItem->setText(0,QString("%1\r\n%2").arg(user->getName()).arg(user->getIp().toString()));
            cItem->setData(0, Qt::UserRole, user->getUuid().toString());
            cItem->setIcon(0, user->getIcon());

            switch (user->getStatus()) {
            case User::OffLine:
                cItem->setBackgroundColor(0, QColor("#ADADAD"));
                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::chatReceiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    mChatRcords->append(ChatRcord(message));
    if(!mChatForms->contains(message.getSenderUuid())) {
        if(mChatRcords->contains()) ////////////////////////////////////
        mesList.insert(message);
        ChatForm *chatForm = new ChatForm(&((*mFriends)[userUuid]), &mesList);
        (*mChatForms)[userUuid] = chatForm;
        chatForm->show();
    }
}

void MainWindow::activated ( QSystemTrayIcon::ActivationReason reason ){
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        qDebug("鼠标单击！");
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug("鼠标双击！");
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug("鼠标中键！");
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent ( QCloseEvent * event ){
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("退出程序"),
        QString(tr("确定要直接退出吗?")),
        QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No) {
        event->ignore();  //忽略退出信号，程序继续运行
    }
    else if (button == QMessageBox::Yes) {
        event->accept();  //接受退出信号，程序退出
    }
}

void MainWindow::createActions()
{
     minimizeAction = new QAction(tr("最小化"), this);
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
     maximizeAction = new QAction(tr("最大化"), this);
     connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
     restoreAction = new QAction(tr("还原"), this);
     connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
     quitAction = new QAction(tr("退出"), this);
     connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}
void MainWindow::createTrayIcon()
{
     trayIconMenu = new QMenu(this);
     trayIconMenu->addAction(minimizeAction);
     trayIconMenu->addAction(maximizeAction);
     trayIconMenu->addAction(restoreAction);
     trayIconMenu->addSeparator();
     trayIconMenu->addAction(quitAction);
     trayIcon->setContextMenu(trayIconMenu);
}
