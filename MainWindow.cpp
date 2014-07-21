#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mIconService(IconService::getService()),
    myself(UserService::getService()->getMyself()),
    mFriends(UserService::getService()->getFriends()),
    mChatRecords(ChatRecordService::getService()->getChatRecords()),
    mChatForms(new QHash<QUuid, ChatForm *>),
    logoIcon(":/images/logo.png")
{
    ui->setupUi(this);

//    setWindowFlags(Qt::FramelessWindowHint);

    (*mFriends)[myself->getUuid()] = *(myself);

    /* chat service */
    mChatService = ChatService::getService();
    connect(mChatService, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)), this, SLOT(chatReceiveSuccess(QHostAddress,quint16,ChatMessage)));

    /* broadcast service*/
    mBroadcastService = BroadcastService::getService();
    ChatMessage reqMes(ChatMessage::Request, myself->getUuid(), myself->toString(), ChatMessage::UserXML);
    mBroadcastService->send(reqMes, QHostAddress("255.255.255.255"));
    connect(mBroadcastService, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(broadcastReceived(QHostAddress,quint16,ChatMessage)));

    /* view */
    ui->userNameLabel->setText(myself->getName());
    ui->userImage->setIconSize(QSize(50, 50));
    ui->userImage->setIcon(mIconService->getIconByUuid(myself->getIconUuid()));
    ui->weatherLabel->setText(QString("<img src=\":/images/weather_1.gif\"/>"));
    ui->searchLabel->setText(QString("<img src=\":/images/search.png\" width='24' height='24'/>"));
    ui->signatureLabel->setText(myself->getInfo());
    ui->contentsTreeWidget->setIconSize(QSize(40, 40));
    connect(ui->contentsTreeWidget,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickedContents(QModelIndex)));
    setStyleSheet("MainWindow { border-image:url(:/images/mainwindow_bg.png);}");
//    setStyleSheet("background-color:#FFFAF0");
    searchResultWidget = new QListWidget(this);
    searchResultWidget->setGeometry(20, 100,265, 60);
    searchResultWidget->setIconSize(QSize(40,40));
    searchResultWidget->setHidden(true);
    connect(searchResultWidget,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickedSearchResult(QModelIndex)));

    /* system tray */
    setWindowIcon(logoIcon);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(logoIcon);
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
    delete ui;
    delete trayIcon;
    delete trayIconMenu;
    delete minimizeAction;
    delete maximizeAction;
    delete restoreAction;
    delete quitAction;
    delete mChatForms;
    delete searchResultWidget;
}

void MainWindow::doubleClickedContents(QModelIndex index){
    if(ui->contentsTreeWidget->currentItem()->childCount() == 0){
        QUuid userUuid = QUuid(index.data(Qt::UserRole).toString());
        openChatForm(userUuid);
    }
}

void MainWindow::openChatForm(const QUuid &receiverUuid){
    if((*mChatForms).contains(receiverUuid)){
        (*mChatForms)[receiverUuid]->setFocus(Qt::ActiveWindowFocusReason);
    } else {
        ChatForm *chatForm = new ChatForm(&((*mFriends)[receiverUuid]));
        (*mChatForms)[receiverUuid] = chatForm;
        connect((*mChatForms)[receiverUuid], SIGNAL(closed(QUuid)), this, SLOT(chatFormClosed(QUuid)));
        chatForm->show();
    }
}

void MainWindow::broadcastReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(message.getContentType() == ChatMessage::UserXML) { // User info
        User user(message.getContent());

        if(user.getIp().isNull() || user.getIp() != senderIp) {
            user.setIp(senderIp);
        }
        if(user.getLogTime().isNull() || user.getLogTime().isValid()) {
            user.setLogTime(QDateTime::currentDateTime());
        }
        if(user.getUuid() != myself->getUuid()) {
            if(user.getStatus() == User::OffLine) {
                MessageDialog *mMessageDialog = new MessageDialog(tr("下线提醒"), QString(tr("%1下线了~")).arg(user.getName()), logoIcon);
                mMessageDialog->show();
            } else {
                MessageDialog *mMessageDialog = new MessageDialog(tr("上线提醒"), QString(tr("%1上线了~")).arg(user.getName()), logoIcon);
                mMessageDialog->show();
            }
        }

        (*mFriends)[message.getSenderUuid()] = user;

        qDebug() << "received" << user.toString();
        updateContentsTreeWidget();

        if(message.getMode() == ChatMessage::Request) {
            ChatMessage myselfMessage(ChatMessage::Response, myself->getUuid(), myself->toString());
            mBroadcastService->send(myselfMessage, senderIp);
        }
    }
}

void MainWindow::updateContentsTreeWidget(){
    ui->contentsTreeWidget->clear();
//    for(int i=0; i<1; i++) {
        User *user;
        QHash<QUuid, User>::iterator it;
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->contentsTreeWidget);
        item->setExpanded(true); // Expand items
        item->setText(0,QString("defualt"));

        for(it = mFriends->begin(); it != mFriends->end(); it++) {
            user = &it.value();
            QTreeWidgetItem *cItem = new QTreeWidgetItem(item);
            cItem->setText(0,QString("%1\r\n%2").arg(user->getName()).arg(user->getIp().toString()));
            cItem->setData(0, Qt::UserRole, user->getUuid().toString());
            cItem->setIcon(0, mIconService->getIconByUuid(user->getIconUuid()));
            if(it.key() == myself->getUuid()) {
                cItem->setBackgroundColor(0, QColor("#FFEBCD"));
            }

            switch (user->getStatus()) {
            case User::OffLine:
                cItem->setBackgroundColor(0, QColor("#ADADAD"));
                break;
            default:
                break;
            }
        }
//    }
}

void MainWindow::chatReceiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(!mFriends->contains(message.getSenderUuid())) {
        qDebug() << "no that friend";
    } else {
        if(!mChatForms->contains(message.getSenderUuid())){
            mChatRecords->append(ChatRecord(message));
            MessageDialog *mMessageDialog = new MessageDialog(tr("新消息提醒！！！"),
                    QString(tr("%1:\r\n%2")).arg((*mFriends)[message.getSenderUuid()].getName()).arg(message.getContent()),
                    mIconService->getIconByUuid((*mFriends)[message.getSenderUuid()].getIconUuid()),
                    10000,
                    message.getSenderUuid());
            mMessageDialog->show();
            connect(mMessageDialog, SIGNAL(accepted(QUuid)), this, SLOT(openChatForm(QUuid)));
        }
    }
}

void MainWindow::fMsgReceiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    mChatRecords->append(ChatRecord(message));
    if(!mFriends->contains(message.getSenderUuid())) {
        qDebug() << "no that friend";
    } else {
        if(!mChatForms->contains(message.getSenderUuid())){
            MessageDialog *mMessageDialog = new MessageDialog(tr("新消息提醒！！！"),
                    QString(tr("%1:\r\n%2")).arg((*mFriends)[message.getSenderUuid()].getName()).arg(FileMessage::fileMessagesToHTMLStr(*FileMessage::parseFileMessages(message.getContent()))),
                    mIconService->getIconByUuid((*mFriends)[message.getSenderUuid()].getIconUuid()),
                    10000,
                    message.getSenderUuid());
            mMessageDialog->show();
            connect(mMessageDialog, SIGNAL(accepted(QUuid)), this, SLOT(openChatForm(QUuid)));
        }
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
        this->isHidden() ? show() : hide();
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug("鼠标中键！");
        break;
    default:
        break;
    }
}

void MainWindow::chatFormClosed(QUuid receiverUuid){
    if(mChatForms->contains(receiverUuid)) {
        mChatForms->remove(receiverUuid);
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

        foreach (QUuid key, mChatForms->keys()) {
            (*mChatForms)[key]->close();
        }
        myself->setStatus(User::OffLine);
        ChatMessage myselfMessage(ChatMessage::Response, myself->getUuid(), myself->toString());
        mBroadcastService->send(myselfMessage, QHostAddress("255.255.255.255"));
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
     connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
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

void MainWindow::on_searchEdit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()){
        searchResultWidget->setHidden(true);
        return;
    }
    QString text = arg1;
    searchResultWidget->clear();
    foreach (QUuid key, mFriends->keys()) {
        User *user = &(*mFriends)[key];
        if(user->getName().contains(text, Qt::CaseInsensitive)
                || user->getIp().toString().contains(text, Qt::CaseInsensitive)){
            QListWidgetItem *item = new QListWidgetItem(searchResultWidget);
            item->setText(QString("%1\r\n%2").arg(user->getName()).arg(user->getIp().toString()));
            item->setData(Qt::UserRole, user->getUuid().toString());
            item->setIcon(mIconService->getIconByUuid(user->getIconUuid()));
        }
    }
    searchResultWidget->resize(searchResultWidget->geometry().width(),
                               44*searchResultWidget->count()<ui->contentsTreeWidget->geometry().height() ?
                                   44*searchResultWidget->count() :
                                   ui->contentsTreeWidget->geometry().height());
    searchResultWidget->setHidden(false);
}

void MainWindow::doubleClickedSearchResult(QModelIndex index){
    QUuid userUuid = QUuid(index.data(Qt::UserRole).toString());
    openChatForm(userUuid);
    searchResultWidget->clear();
    searchResultWidget->setHidden(true);
}

void MainWindow::on_contentsTreeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *popMenu = new QMenu(this);
    popMenu->addAction(ui->actionNewFriendSet); // 往菜单内添加QAction   该action在前面用设计器定义了
    popMenu->addAction(ui->actionDeleteFriendSet);
    popMenu->exec(QCursor::pos()); // 弹出右键菜单，菜单位置为光标位置
}

