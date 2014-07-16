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
}

void ChatForm::initForm(){
    setAttribute(Qt::WA_DeleteOnClose);
    this->mChatService = ChatService::getService();

    ui->userIcon->setIcon(mIconService->getIconByUuid(this->receiver->getIconUuid()));
    ui->userIcon->setIconSize(QSize(40, 40));

    ui->usernameLabel->setText(this->receiver->getName());
    ui->signatrueLabel->setText(this->receiver->getInfo());

    setStyleSheet("#ChatForm {background-color:#ffdbf8}");

    connect(mChatService, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)), this, SLOT(receiveSuccess(QHostAddress,quint16,ChatMessage)));
    connect(mChatService, SIGNAL(sendError(QUuid, QString)), this, SLOT(sendError(QUuid, QString)));
    connect(mChatService, SIGNAL(sendSuccess(QUuid)), this, SLOT(sendSuccess(QUuid)));

}

ChatForm::~ChatForm()
{
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
