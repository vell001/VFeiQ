#include "ChatForm.h"
#include "ui_ChatForm.h"

ChatForm::ChatForm(User *receiver, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatForm),
    receiver(receiver),
    sender(UserService::getService()->getMyself()),
    mChatRecords(ChatRecordService::getService()->getChatRecordsByUserUuid(receiver->getUuid(), ChatRecord::NotRead))
{
    ui->setupUi(this);
    initForm();
    updateChatRecordView();
    loadSetting();
    sendFileModel = new QStandardItemModel;
    QStringList heads;
    heads << "filename" << "progress" << "size" << "type" << "url" << "uuid";
    sendFileModel->setHorizontalHeaderLabels(heads);
    ui->fileTableView->setModel(sendFileModel);
    mFileSender = 0;
    mFileReceiver = 0;
    mFileMessage = 0;
}

void ChatForm::initForm(){
    setAttribute(Qt::WA_DeleteOnClose);
    this->mChatService = ChatService::getService(9514);
    this->mFileMsgService = BroadcastService::getService(9323);

    ui->userIcon->setIcon(mIconService->getIconByUuid(this->receiver->getIconUuid()));
    ui->userIcon->setIconSize(QSize(40, 40));

    ui->usernameLabel->setText(this->receiver->getName());
    ui->signatrueLabel->setText(this->receiver->getInfo());

    setStyleSheet("#ChatForm {background-color:#ffdbf8}");

    connect(mChatService, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)), this, SLOT(receiveSuccess(QHostAddress,quint16,ChatMessage)));
    connect(mChatService, SIGNAL(sendError(QUuid, QString)), this, SLOT(sendError(QUuid, QString)));
    connect(mChatService, SIGNAL(sendSuccess(QUuid)), this, SLOT(sendSuccess(QUuid)));
    connect(mFileMsgService, SIGNAL(received(QHostAddress,quint16,ChatMessage)), this, SLOT(fileMsgReceived(QHostAddress,quint16,ChatMessage)));
}

ChatForm::~ChatForm()
{
    delete mFileSender;
    delete mFileReceiver;
    delete mFileMessage;
//    qDebug() << "ChatForm::~ChatForm()";
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
    ChatMessage message(ChatMessage::Request, sender->getUuid(), ui->messageTextEdit->toHtml(), this);
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
        ui->fileTableView->setIndexWidget(index, new QProgressBar);
        QProgressBar *bar = (QProgressBar *)ui->fileTableView->indexWidget(index);
    }
}

void ChatForm::on_cancelSendButton_clicked()
{
    clearSendFiles();
}

void ChatForm::on_sendFileButton_clicked()
{
    fileSendRow = -1;
    if(sendFileModel->rowCount() < 1) {
        QMessageBox::warning(this, "send warning", "no file chose to send");
    } else {
        fileSendEnd(0);
    }
}

void ChatForm::fileMsgReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    if(message.getType() == ChatMessage::Request){
        FileMessage fMsg(message.getContent());
        if(QMessageBox::question(this, "file transfer", "do you want to save those files?", QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok){

            QString saveFileName = QFileDialog::getSaveFileName(this, fMsg.getFileName());
            if(saveFileName.isEmpty()){
                ChatMessage mes(ChatMessage::Response, sender->getUuid(), "unaccepted");
                mFileMsgService->send(mes, receiver->getIp());
                return;
            }
            delete mFileReceiver;
            fMsg.setTransferPort(1213);
            mFileReceiver = new FileReceiver(new QFile(saveFileName),
                                             QHostAddress::Any,
                                             fMsg.getTransferPort(),
                                             fMsg.getUuid());
            connect(mFileReceiver, SIGNAL(receiveEnd(qint64)), this, SLOT(fileReceiveEnd(qint64)));
            connect(mFileReceiver, SIGNAL(receiveError(QString)), this, SLOT(fileReceiveError(QString)));
            connect(mFileReceiver, SIGNAL(receiveProgress(qint64,qint64)), this, SLOT(fileReceiveProgress(qint64,qint64)));
//            mFileReceiver->setUuid(fMsg.getUuid());
//            mFileReceiver->setFile(new QFile(saveFileName));
//            mFileReceiver->setSenderIp(QHostAddress::Any); ////////////////////
//            mFileReceiver->setReceivePort(fMsg.getTransferPort());
            mFileReceiver->start();

            ChatMessage mes(ChatMessage::Response, sender->getUuid(), fMsg.toString());
            mFileMsgService->send(mes, receiver->getIp());

        } else {
            ChatMessage mes(ChatMessage::Response, sender->getUuid(), "unaccepted");
            mFileMsgService->send(mes, receiver->getIp());
        }
    } else if(message.getType() == ChatMessage::Response) {
        if(message.getContent() == "unaccepted") { // 对方拒绝接受
            clearSendFiles();
        } else {
            FileMessage fMsg(message.getContent());

            delete mFileSender;
            mFileSender = new FileSender(new QFile(mFileMessage->getFileFullName()),
                                         receiver->getIp(),
                                         fMsg.getTransferPort(),
                                         fMsg.getUuid());
            connect(mFileSender, SIGNAL(sendEnd(qint64)), this, SLOT(fileSendEnd(qint64)));
            connect(mFileSender, SIGNAL(sendError(QString)), this, SLOT(fileSendError(QString)));
            connect(mFileSender, SIGNAL(sendProgress(qint64,qint64)), this, SLOT(fileSendProgress(qint64,qint64)));
//            mFileSender->setUuid(fMsg.getUuid());
//            mFileSender->setFile(new QFile(mFileMessage->getFileFullName()));
//            mFileSender->setReceiverIp(receiver->getIp());
//            mFileSender->setReceiverPort(fMsg.getTransferPort());
            mFileSender->send();
        }
    }
}

void ChatForm::fileSendEnd(qint64 btyes){
    if(fileSendRow+1 < sendFileModel->rowCount()) {
        fileSendRow++;
        delete mFileMessage;
        mFileMessage = new FileMessage(QFileInfo(sendFileModel->index(fileSendRow, 4).data(Qt::DisplayRole).toString()));
        QUuid fUuid(QUuid(sendFileModel->index(fileSendRow, 5).data(Qt::DisplayRole).toString()));
        ChatMessage fileMsg(fUuid, ChatMessage::Request, sender->getUuid(), mFileMessage->toString());
        mFileMsgService->send(fileMsg, receiver->getIp());
    } else {
        QMessageBox::information(this, "send ok", "send finish");
        clearSendFiles();
    }
}

void ChatForm::fileSendError(QString errorStr){
     QMessageBox::warning(this, "send error", errorStr);
}

void ChatForm::fileSendProgress(qint64 maximum, qint64 value){
    QModelIndex progressBarIndex = sendFileModel->index(fileSendRow, 1);
    QProgressBar *bar = (QProgressBar *)ui->fileTableView->indexWidget(progressBarIndex);
    bar->setMaximum(maximum);
    bar->setValue(value);
}

void ChatForm::fileReceiveEnd(qint64 bytes){

}

void ChatForm::fileReceiveError(QString errorStr){

}

void ChatForm::fileReceiveProgress(qint64 maximum, qint64 value){

}

void ChatForm::clearSendFiles(){
    sendFileModel->removeRows(0, sendFileModel->rowCount());
}
