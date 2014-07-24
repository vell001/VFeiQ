#include "ChatForm.h"
#include "ui_ChatForm.h"

ChatForm::ChatForm(User *receiver, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatForm),
    receiver(receiver),
    sender(UserService::getService()->getMyself()),
    mChatRecords(ChatRecordService::getService()->getChatRecordsByUserUuid(receiver->getUuid(), ChatRecord::NotRead)),
    mIconService(IconService::getService())
{
    ui->setupUi(this);

    /* recent friends */
    UserService::getService()->insertRecentFriend(*receiver);
    /* end of recent friends */

    setAttribute(Qt::WA_DeleteOnClose);
    this->mChatService = ChatService::getService();
    this->mFileMsgService = FileMessageService::getService();

    ui->userIcon->setIcon(mIconService->getUserIconByUuid(this->receiver->getIconUuid()));
    ui->userIcon->setIconSize(QSize(40, 40));

    ui->usernameLabel->setText(this->receiver->getName());
    ui->signatrueLabel->setText(this->receiver->getInfo());

    setStyleSheet("#ChatForm {background-color:#ffdbf8}");

    connect(mChatService, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)), this, SLOT(receiveSuccess(QHostAddress,quint16,ChatMessage)));
    connect(mChatService, SIGNAL(sendError(QUuid, QString)), this, SLOT(sendError(QUuid, QString)));
    connect(mChatService, SIGNAL(sendSuccess(QUuid)), this, SLOT(sendSuccess(QUuid)));
    connect(mFileMsgService, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(fileMsgReceived(QHostAddress,quint16,ChatMessage)));

    updateChatRecordView();
    loadSetting();

    sendFileModel = new QStandardItemModel;
    receiveFileModel = new QStandardItemModel;

    QStringList heads;
    heads << "filename" << "progress" << "size" << "type" << "url" << "uuid";
    sendFileModel->setHorizontalHeaderLabels(heads);
    receiveFileModel->setHorizontalHeaderLabels(heads);

    ui->sendFileTableView->setModel(sendFileModel);
    ui->receiveFileTableView->setModel(receiveFileModel);

    mFileSender = 0;
    mFileReceiver = 0;
    mFileMessage = 0;

    fileSendRow = 0;
    fileReceiveRow = 0;

    mShakeTimer = new QTimer(this);
    mShakeTimer->setInterval(30);
    mShakePosition = 0;
    shakeMaxLimitTimes = 12;
    shakeMaxLimitSpace = 5;
    connect(mShakeTimer, SIGNAL(timeout()),this, SLOT(shakeTimeOut()));
}

ChatForm::~ChatForm()
{
    delete mFileSender;
    delete mFileReceiver;
    delete mFileMessage;
    delete sendFileModel;
    delete receiveFileModel;
    delete ui;
}

void ChatForm::closeEvent ( QCloseEvent * event ){
    emit closed(receiver->getUuid());
    event->accept();
}

void ChatForm::on_sendButton_clicked()
{
    sendMessage();
}

void ChatForm::on_closeButton_clicked()
{
    this->close();
}

void ChatForm::sendError(QUuid messageUuid , QString errorMessage){
    qDebug() << "sendError: " << messageUuid.toString() << " : " << errorMessage;
}

void ChatForm::sendSuccess(QUuid messageUuid){
    qDebug() << "send ok";
}

void ChatForm::receiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    qDebug() << message.getSenderUuid() << receiver->getUuid();
    if(message.getSenderUuid() == receiver->getUuid()) {
        if(message.getContent() == ":/shake") {
            startShake();
        }
        ChatRecord *record = new ChatRecord(message);
        mChatRecords.append(record);
        updateChatRecordView();
    }
}

void ChatForm::keyPressEvent(QKeyEvent *e){
    qDebug() << "key: " << e->key();
//    switch (e->key()) {
//    case Qt::Key_Enter :
//        qDebug() << "entered";
//        sendMessage();
//        break;
//    default:
//        break;
//    }
}

void ChatForm::sendMessage() {
    ChatMessage message(ChatMessage::Request, sender->getUuid(), ui->messageTextEdit->toHtml());
    mChatService->send(message, receiver->getIp());
    ChatRecord *record = new ChatRecord(message);
    mChatRecords.append(record);
    updateChatRecordView();
    ui->messageTextEdit->clear();
}

void ChatForm::updateChatRecordView(){
//    qSort(mChatRecords);
    QString RecordText("<html><body>");
    foreach (ChatRecord *record, mChatRecords) {

        RecordText += QString("<div id=\"%1\" name=\"chatRecord\">")
                .arg(record->getUuid().toString());

        if(record->getSendOrReceive() == ChatRecord::Send) {
            RecordText += QString("<div name=\"info\" style=\"color:#FF9900;font-size:16px;\">%1:&nbsp;%2</div>")
                    .arg(sender->getName())
                    .arg(record->getTime().time().toString());
        } else {
            RecordText += QString("<div name=\"info\" style=\"color:#9933E5;font-size:16px;\">%1:&nbsp;%2</div>")
                    .arg(receiver->getName())
                    .arg(record->getTime().time().toString());
        }

        RecordText += QString("<div name=\"content\">&nbsp;%1</div>")
                .arg(record->getContent());

        RecordText += QString("</div>");

        record->setIsRead(ChatRecord::Readed);
    }
    RecordText += QString("</body></html>");
    ui->chatRecordBrowser->setHtml(RecordText);
    QTextCursor cursor = ui->chatRecordBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->chatRecordBrowser->setTextCursor(cursor);
}

void ChatForm::on_fontButton_clicked()
{
    bool ok = false;
    QFont textFont = QFontDialog::getFont(&ok, ui->messageTextEdit->font());
    if(ok) {
        ui->messageTextEdit->setFont(textFont);
        saveSetting();
    }
}

void ChatForm::on_colorButton_clicked()
{
    QColor textColor = QColorDialog::getColor(ui->messageTextEdit->textColor());
    ui->messageTextEdit->setTextColor(textColor);
    saveSetting();
}

void ChatForm::on_facesButton_clicked()
{

}

void ChatForm::loadSetting(){
    QSettings setting("vell001", "VFeiQ");
    setting.beginGroup("ChatForm");
    QColor textColor = setting.value("textColor", QVariant::fromValue(QColor("blue"))).value<QColor>();
    QFont textFont = setting.value("textFont").value<QFont>();
    ui->messageTextEdit->setTextColor(textColor);
    ui->messageTextEdit->setFont(textFont);
    setting.endGroup();
}

void ChatForm::saveSetting(){
    QSettings setting("vell001", "VFeiQ");
    setting.beginGroup("ChatForm");
    setting.setValue("textColor", QVariant::fromValue(ui->messageTextEdit->textColor()));
    setting.setValue("textFont", QVariant::fromValue(ui->messageTextEdit->font()));
    setting.endGroup();
}

void ChatForm::on_choseFileButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames();

//    heads << "filename" << "progress" << "size" << "type" << "url" << "uuid";
    foreach (QString fileName, fileNames) {
        QFileInfo fileInfo(fileName);
        QString type = fileInfo.isDir() ? "dir" : "file";

        QList<QStandardItem *> items;
        items << new QStandardItem(fileInfo.fileName())
              << new QStandardItem
              << new QStandardItem(QString::number(fileInfo.size()))
              << new QStandardItem(type)
              << new QStandardItem(fileName)
              << new QStandardItem(QUuid::createUuid().toString());
        sendFileModel->appendRow(items);
    }

    for(int i=0; i<sendFileModel->rowCount(); i++){
        QModelIndex index = sendFileModel->index(i,1);
        ui->sendFileTableView->setIndexWidget(index, new QProgressBar);
    }
}

void ChatForm::on_cancelSendButton_clicked()
{
    clearSendFiles();
}

void ChatForm::on_sendFileButton_clicked()
{
    if(sendFileModel->rowCount() < 1) {
        QMessageBox::warning(this, "send warning", "no file chose to send");
    } else {
//        sendFilesMessage
        QHash<QUuid, FileMessage *> fileMessages;
        FileMessage *fileMessage;
        for(int i=0; i<sendFileModel->rowCount(); i++){
            fileMessage = new FileMessage(QFileInfo(sendFileModel->index(i, 4).data(Qt::DisplayRole).toString()));
            fileMessages[fileMessage->getUuid()] = fileMessage;
        }
        ChatMessage fileMsg(ChatMessage::Request, sender->getUuid(), FileMessage::fileMessagesToXMLStr(fileMessages), ChatMessage::FilesXML);
        mFileMsgService->send(fileMsg, receiver->getIp());

        // add a filemessage record
        ChatMessage fileMsgHtml(ChatMessage::Request, sender->getUuid(), "send files: "+FileMessage::fileMessagesToHTMLStr(fileMessages));
        ChatRecord *record = new ChatRecord(fileMsgHtml);
        mChatRecords.append(record);
        updateChatRecordView();
    }
    fileSendRow = 0;
}

void ChatForm::fileMsgReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(message.getMode() == ChatMessage::Request){ // someone want to transfer files to you
        if(message.getContentType() != ChatMessage::FilesXML && message.getContentType() != ChatMessage::FileXML)
            return ;
        QHash<QUuid, FileMessage *> *fileMessages = FileMessage::parseFileMessages(message.getContent());
//    heads << "filename" << "progress" << "size" << "type" << "url" << "uuid";
        // init receive table
        foreach (FileMessage *fileMessage, fileMessages->values()) {
            QList<QStandardItem *> items;
            items << new QStandardItem(fileMessage->getFileName())
                  << new QStandardItem
                  << new QStandardItem(QString::number(fileMessage->getSize()))
                  << new QStandardItem(fileMessage->getType() == FileMessage::Dir ? "dir" : "file")
                  << new QStandardItem(fileMessage->getFileFullName())
                  << new QStandardItem(fileMessage->getUuid().toString());
            receiveFileModel->appendRow(items);
        }
        for(int i=0; i<receiveFileModel->rowCount(); i++){
            QModelIndex index = receiveFileModel->index(i,1);
            ui->receiveFileTableView->setIndexWidget(index, new QProgressBar);
        }
        fileReceiveRow = 0;

    } else if(message.getMode() == ChatMessage::Response) {
        if(message.getContent() == "unaccepted") { // 对方拒绝接受
            clearSendFiles();
        } else {
            FileMessage fMsg(message.getContent());

            delete mFileSender;
            mFileSender = new FileSender(new QFile(fMsg.getFileFullName()),
                                         receiver->getIp(),
                                         fMsg.getTransferPort(),
                                         fMsg.getUuid());
            connect(mFileSender, SIGNAL(sendEnd(qint64)), this, SLOT(fileSendEnd(qint64)));
            connect(mFileSender, SIGNAL(sendError(QString)), this, SLOT(fileSendError(QString)));
            connect(mFileSender, SIGNAL(sendProgress(qint64,qint64)), this, SLOT(fileSendProgress(qint64,qint64)));

            mFileSender->send();
        }
    }
}

void ChatForm::fileSendEnd(qint64 btyes){
    if(fileSendRow+1 < sendFileModel->rowCount()) {
        fileSendRow++;
    } else { // send over
        clearSendFiles();
        ChatMessage msg(ChatMessage::Response, receiver->getUuid(), "send finish");
        ChatRecord *record = new ChatRecord(msg);
        mChatRecords.append(record);
        updateChatRecordView();
    }
}

void ChatForm::fileSendError(QString errorStr){
     QMessageBox::warning(this, "send error", errorStr);
     ChatMessage msg(ChatMessage::Response, receiver->getUuid(), "send error: "+errorStr);
     ChatRecord *record = new ChatRecord(msg);
     mChatRecords.append(record);
     updateChatRecordView();
}

void ChatForm::fileSendProgress(qint64 maximum, qint64 value){
    QModelIndex progressBarIndex = sendFileModel->index(fileSendRow, 1);
    QProgressBar *bar = (QProgressBar *)ui->sendFileTableView->indexWidget(progressBarIndex);
    bar->setMaximum(maximum);
    bar->setValue(value);
}

void ChatForm::fileReceiveEnd(qint64 bytes){
    if(fileReceiveRow+1 < receiveFileModel->rowCount()){
        fileReceiveRow++;
        delete mFileMessage;
        QUuid fUuid(QUuid(receiveFileModel->index(fileReceiveRow, 5).data(Qt::DisplayRole).toString()));
        mFileMessage = new FileMessage(QFileInfo(receiveFileModel->index(fileReceiveRow, 4).data(Qt::DisplayRole).toString()), fUuid);

        // start receiver
        delete mFileReceiver;
        mFileMessage->setTransferPort(1213);
        mFileReceiver = new FileReceiver(new QFile(saveDirName+"/"+mFileMessage->getFileName()),
                                         QHostAddress::Any,
                                         mFileMessage->getTransferPort(),
                                         mFileMessage->getUuid());
        connect(mFileReceiver, SIGNAL(receiveEnd(qint64)), this, SLOT(fileReceiveEnd(qint64)));
        connect(mFileReceiver, SIGNAL(receiveError(QString)), this, SLOT(fileReceiveError(QString)));
        connect(mFileReceiver, SIGNAL(receiveProgress(qint64,qint64)), this, SLOT(fileReceiveProgress(qint64,qint64)));

        mFileReceiver->start();

        ChatMessage fileMsg(fUuid, ChatMessage::Response, sender->getUuid(), mFileMessage->toString(), ChatMessage::FileXML);
        mFileMsgService->send(fileMsg, receiver->getIp());
    } else { // receive over
        clearReceiveFiles();
        ChatMessage msg(ChatMessage::Response, receiver->getUuid(), "receive finish<br/><a href=\"openDir?dir="+saveDirName+"\">open save dir</a>");
        ChatRecord *record = new ChatRecord(msg);
        mChatRecords.append(record);
        updateChatRecordView();
    }
}

void ChatForm::fileReceiveError(QString errorStr){
    QMessageBox::warning(this, "receive error", errorStr);
    ChatMessage msg(ChatMessage::Response, receiver->getUuid(), "receive error: "+errorStr);
    ChatRecord *record = new ChatRecord(msg);
    mChatRecords.append(record);
    updateChatRecordView();
}

void ChatForm::fileReceiveProgress(qint64 maximum, qint64 value){
    QModelIndex progressBarIndex = receiveFileModel->index(fileReceiveRow, 1);
    QProgressBar *bar = (QProgressBar *)ui->receiveFileTableView->indexWidget(progressBarIndex);
    bar->setMaximum(maximum);
    bar->setValue(value);
}

void ChatForm::clearSendFiles(){
    sendFileModel->removeRows(0, sendFileModel->rowCount());
}
void ChatForm::clearReceiveFiles(){
    receiveFileModel->removeRows(0, receiveFileModel->rowCount());
}

void ChatForm::on_saveReceiveButton_clicked()
{
    saveDirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                            saveDirName,
                                                            QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
    if(saveDirName.isEmpty()){
        QMessageBox::warning(this, "directory error", "directory is not exist");
        return;
    } else { // start receive

        fileReceiveRow = -1;
        fileReceiveEnd(0);
    }
}

void ChatForm::on_cancelReceiveButton_clicked()
{
    ChatMessage mes(ChatMessage::Response, sender->getUuid(), "unaccepted");
    mFileMsgService->send(mes, receiver->getIp());
    clearReceiveFiles();
}

void ChatForm::on_chatRecordBrowser_anchorClicked(const QUrl &url)
{
    QList<QPair<QString, QString> > items = url.queryItems();
    if(url.path() == "openDir") {  // openDir
        for(int i=0; i<items.size(); i++){
            QPair<QString, QString> *dirPair = &items[i];
            if(dirPair->first == "dir") {
                QDesktopServices::openUrl(QUrl("file:///" + dirPair->second, QUrl::TolerantMode));
            }
        }
        return;
    } else if(QMessageBox::question(this, "open url", "do you want to open this url?", QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok){
        QDesktopServices::openUrl(url);
    }
}

void ChatForm::on_shakeButton_clicked()
{
    ChatMessage message(ChatMessage::Request, sender->getUuid(), ":/shake");
    mChatService->send(message, receiver->getIp());
    ChatRecord *record = new ChatRecord(message);
    mChatRecords.append(record);
    updateChatRecordView();
    ui->messageTextEdit->clear();
}

void ChatForm::startShake(){
    mShakePosition = 0;
    mShakeCurPos = this->pos();
    mShakeTimer->start();
}

void ChatForm::shakeTimeOut(){
    mShakeTimer->stop();
    if(mShakePosition < shakeMaxLimitTimes)
    {
        ++mShakePosition;
        switch(mShakePosition%4)
        {
        case 1:
        {
            QPoint tmpPos(mShakeCurPos.x(),mShakeCurPos.y()-shakeMaxLimitSpace);
            this->move(tmpPos);
        }
            break;
        case 2:
        {
            QPoint tmpPos(mShakeCurPos.x()-shakeMaxLimitSpace,mShakeCurPos.y()-shakeMaxLimitSpace);
            this->move(tmpPos);
        }
            break;
        case 3:
        {
            QPoint tmpPos(mShakeCurPos.x()-shakeMaxLimitSpace,mShakeCurPos.y());
            this->move(tmpPos);
        }
            break;
        default:
        case 0:
            this->move(mShakeCurPos);
            break;
        }
        mShakeTimer->start();
    }
}

void ChatForm::on_sharedFilesButton_clicked()
{
    SharedFilesDialog *sharedFilesDialog = new SharedFilesDialog(receiver);
    sharedFilesDialog->show();
}
