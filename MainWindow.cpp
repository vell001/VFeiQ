#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mIconService(IconService::getService()),
    mUserService(UserService::getService()),
    mUserInfoService(UserInfoService::getService()),
    mChatService(ChatService::getService()),
    mFileShareService(FileShareService::getService()),
    myself(UserService::getService()->getMyself()),
    mFriends(UserService::getService()->getFriends()),
    mChatRecords(ChatRecordService::getService()->getChatRecords()),
    mChatForms(new QHash<QUuid, ChatForm *>),
    logoIcon(":/images/logo.png")
{
    ui->setupUi(this);

//    setWindowFlags(Qt::FramelessWindowHint);

    /* chat service */
    connect(mChatService, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)), this, SLOT(chatReceiveSuccess(QHostAddress,quint16,ChatMessage)));

    /* iserInfo service*/
    mUserInfoService->sendMyselfInfo();
    connect(mUserInfoService, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(userInfoReceived(QHostAddress,quint16,ChatMessage)));

    /* user service */
    connect(mUserService, SIGNAL(myselfInfoChanged(User*)), mUserInfoService, SLOT(sendMyselfInfo()));
    (*mFriends)[myself->getUuid()] = *(myself);

    /* view */
    ui->userNameLabel->setText(myself->getName());
    ui->userImage->setIconSize(QSize(50, 50));
    ui->userImage->setIcon(mIconService->getUserIconByUuid(myself->getIconUuid()));
    ui->weatherLabel->setText(QString("<img src=\":/images/weather_1.gif\"/>"));
    ui->searchLabel->setText(QString("<img src=\":/images/search.png\" width='24' height='24'/>"));
    ui->signatureLabel->setText(myself->getInfo());
    ui->contentsTreeWidget->setIconSize(QSize(40, 40));
    ui->recentContentsListWidget->setIconSize(QSize(40, 40));

    connect(ui->contentsTreeWidget,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickedContents(QModelIndex)));

    connect(ui->recentContentsListWidget,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickedRecentContents(QModelIndex)));
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

void MainWindow::userInfoReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(message.getContentType() == ChatMessage::UserXML) { // User info
        User user(message.getContent());

        if(user.getIp().isNull() || user.getIp() != senderIp) {
            user.setIp(senderIp);
        }
        if(user.getLogTime().isNull() || user.getLogTime().isValid()) {
            user.setLogTime(QDateTime::currentDateTime());
        }
        if(!mIconService->containsUserIcon(user.getIconUuid())) {
            mIconService->addIconToGet(user.getIconUuid(), senderIp);
        }
        if(user.getUuid() != myself->getUuid()) { // others info
            if(user.getStatus() == User::Offline) {
                MessageDialog *mMessageDialog = new MessageDialog(tr("下线提醒"), QString(tr("%1下线了~")).arg(user.getName()), logoIcon);
                mMessageDialog->show();
            } else if(!mFriends->contains(user.getUuid())
                      || (*mFriends)[user.getUuid()].getStatus() == User::Offline){
                MessageDialog *mMessageDialog = new MessageDialog(tr("上线提醒"), QString(tr("%1上线了~")).arg(user.getName()), logoIcon);
                mMessageDialog->show();
            }
        } else { // myself info
            updateMyselfInfoView();
        }

        if(mFriends->contains(user.getUuid())) {
            if((*mFriends)[user.getUuid()] != user){
                qDebug() << "user info changed";
            }
        }
        (*mFriends)[user.getUuid()] = user;

        qDebug() << "received: " << user.toString();
        updateContentsTreeWidget();

        if(message.getMode() == ChatMessage::Request) {
            ChatMessage myselfMessage(ChatMessage::Response, myself->getUuid(), myself->toString(), ChatMessage::UserXML);
            mUserInfoService->send(myselfMessage, senderIp);
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
            cItem->setText(0,QString("%1 [%2]\r\n%3")
                           .arg(user->getName())
                           .arg(user->getStatusStr())
                           .arg(user->getIp().toString()));
            cItem->setData(0, Qt::UserRole, user->getUuid().toString());
            cItem->setIcon(0, mIconService->getUserIconByUuid(user->getIconUuid()));
            if(it.key() == myself->getUuid()) {
                cItem->setBackgroundColor(0, QColor("#FFEBCD"));
            }

            switch (user->getStatus()) {
            case User::Offline:
                cItem->setBackgroundColor(0, QColor("#ADADAD"));
                break;
            default:
                break;
            }
        }
//    }
}

void MainWindow::updateMyselfInfoView(){
    ui->userImage->setIcon(mIconService->getUserIconByUuid(myself->getIconUuid()));
    ui->userNameLabel->setText(myself->getName());
    ui->signatureLabel->setText(myself->getInfo());
}

void MainWindow::chatReceiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(!mFriends->contains(message.getSenderUuid())) {
        qDebug() << "no that friend";
    } else {
        if(!mChatForms->contains(message.getSenderUuid())){
            mChatRecords->append(ChatRecord(message));
            MessageDialog *mMessageDialog = new MessageDialog(tr("新消息提醒！！！"),
                    QString(tr("%1:\r\n%2")).arg((*mFriends)[message.getSenderUuid()].getName()).arg(message.getContent()),
                    mIconService->getUserIconByUuid((*mFriends)[message.getSenderUuid()].getIconUuid()),
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
                    mIconService->getUserIconByUuid((*mFriends)[message.getSenderUuid()].getIconUuid()),
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
        myself->setStatus(User::Offline);
        mUserInfoService->sendMyselfInfo();
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
            item->setIcon(mIconService->getUserIconByUuid(user->getIconUuid()));
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

/* share files */
void MainWindow::on_setShareFilesButton_clicked()
{
    SetShareFilesDialog *mSetShareFilesDialog = new SetShareFilesDialog;
    mSetShareFilesDialog->show();
}

/* end of share files */

/* recent friends */
void MainWindow::updateRecentFriendsListWidget() {
    ui->recentContentsListWidget->clear();
    QHash<QString, User> *recentFriends = mUserService->getRecentFriends();

    QList<QString> keys = recentFriends->keys();

    QList<QDateTime> times;
    foreach (QString timeStr, keys) {
        times.push_back(QDateTime::fromString(timeStr));
    }

    foreach (QDateTime time, times) {
        User *user = &(*recentFriends)[time.toString()];
        QListWidgetItem *item = new QListWidgetItem(ui->recentContentsListWidget);
        item->setIcon(mIconService->getUserIconByUuid(user->getIconUuid()));
        item->setText(QString("%1\r\n%2").arg(user->getName()).arg(time.toString()));
        item->setData(Qt::UserRole, user->getUuid().toString());

        switch (user->getStatus()) {
        case User::Offline:
            item->setBackgroundColor(QColor("#ADADAD"));
            break;
        default:
            break;
        }
    }
}

void MainWindow::doubleClickedRecentContents(QModelIndex index){
    QUuid userUuid = QUuid(index.data(Qt::UserRole).toString());
    openChatForm(userUuid);
}

void MainWindow::on_cleanChatRecordButton_clicked()
{
    mUserService->getRecentFriends()->clear();
    updateRecentFriendsListWidget();
}

/* end of recent friends */

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 2) {
        updateRecentFriendsListWidget();
    }
}

void MainWindow::on_userImage_clicked()
{
    SettingDialog *settingDialog = SettingDialog::getDialog();
    settingDialog->setCurrentIndex(SettingDialog::UserInfo);
    settingDialog->show();
}

void MainWindow::on_settingButton_clicked()
{
    SettingDialog *settingDialog = SettingDialog::getDialog();
    settingDialog->setCurrentIndex(SettingDialog::UserInfo);
    settingDialog->show();
}

void MainWindow::on_statusComboBox_currentIndexChanged(int index)
{
    myself->setStatus((User::Status)index);
    mUserService->saveMyself();

    ChatMessage myselfMsg(ChatMessage::Response, myself->getUuid(), myself->toString(), ChatMessage::UserXML);
    mUserInfoService->broadcast(myselfMsg);
}

void MainWindow::on_gamesButton_clicked()
{
    GamesDialog *gamesDialog = new GamesDialog;
    gamesDialog->show();
}
