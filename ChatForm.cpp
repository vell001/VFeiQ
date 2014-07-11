#include "ChatForm.h"
#include "ui_ChatForm.h"

ChatForm::ChatForm(const User &receiver, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatForm)
{
    ui->setupUi(this);
    this->receiver = receiver;
    this->mChatService = ChatService::getService();
    connect(mChatService, SIGNAL(receiveSuccess(QHostAddress,quint16,ChatMessage)), this, SLOT(receiveSuccess(QHostAddress,quint16,ChatMessage)));
    connect(mChatService, SIGNAL(sendError(QUuid, QString)), this, SLOT(sendError(QUuid, QString)));
    connect(mChatService, SIGNAL(sendSuccess(QUuid)), this, SLOT(sendSuccess(QUuid)));
}

ChatForm::~ChatForm()
{
    delete ui;
}

void ChatForm::on_sendButton_clicked()
{
    sendMessage();
}

void ChatForm::sendError(QUuid messageUuid , QString errorMessage){
    qDebug() << "sendError: " << messageUuid.toString() << " : " << errorMessage;
}

void ChatForm::sendSuccess(QUuid messageUuid){
    qDebug() << "messageUuid : " << messageUuid.toString();
    QListWidgetItem *mesItem;
    for(int i=ui->chatListWidget->count()-1; i>=0; i--) {
        mesItem = ui->chatListWidget->item(i);
        qDebug() << mesItem->text();
        qDebug() << "mesItem : " << mesItem->data(Qt::UserRole).toString();
        if(messageUuid.toString() == mesItem->data(Qt::UserRole).toString()) {
            mesItem->setBackgroundColor(QColor("#FFF68F"));
            return ;
        }
    }
    qDebug() << "send ok";
}

void ChatForm::receiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    QListWidgetItem *mesItem = new QListWidgetItem(ui->chatListWidget);
    mesItem->setData(Qt::UserRole, message.getUuid().toString());
    mesItem->setText(QString("%1:\r\n%2").arg(receiver.getName()).arg(QString(message.getContent())));
    mesItem->setBackgroundColor(QColor("#F0E68C"));
    ui->chatListWidget->setCurrentItem(mesItem);
    qDebug() << "receiveSuccess: " << senderIp << senderPort;
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
    ChatMessage message(ChatMessage::Request, receiver.getUuid(), ui->messagePlainTextEdit->toPlainText().toUtf8(), this);
    mChatService->send(message, receiver.getIp());
    QListWidgetItem *mesItem = new QListWidgetItem(ui->chatListWidget);
    mesItem->setData(Qt::UserRole, message.getUuid().toString());
    mesItem->setText(QString("%1:\r\n").arg(SettingUtil::getUtil()->getSender()->getName()).append(message.getContent()));
    ui->messagePlainTextEdit->clear();
}
