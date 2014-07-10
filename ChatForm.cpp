#include "ChatForm.h"
#include "ui_ChatForm.h"

ChatForm::ChatForm(const User &receiver, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatForm)
{
    ui->setupUi(this);
    this->receiver = receiver;
    this->mChatService = ChatService::getService();
    connect(mChatService, SIGNAL(receiveSuccess(ChatMessage)), this, SLOT(receiveSuccess(ChatMessage)));
    connect(mChatService, SIGNAL(sendError(QUuid, QString)), this, SLOT(sendError(QUuid, QString)));
    connect(mChatService, SIGNAL(sendSuccess(QUuid)), this, SLOT(sendSuccess(QUuid)));
}

ChatForm::~ChatForm()
{
    delete ui;
}

void ChatForm::on_sendButton_clicked()
{
    // send message;
    sendMessage();
}

void ChatForm::sendError(QUuid messageUuid , QString errorMessage){

}

void ChatForm::sendSuccess(QUuid messageUuid){
    qDebug() << "messageUuid : " << messageUuid.toString();
    QListWidgetItem *mesItem;
    for(int i=ui->chatListWidget->count()-1; i>=0; i--) {
        mesItem = ui->chatListWidget->item(i);
        qDebug() << mesItem->text();
        qDebug() << "mesItem : " << mesItem->data(Qt::UserRole).toString();
        if(messageUuid.toString() == mesItem->data(Qt::UserRole).toString()) {
            mesItem->setBackgroundColor(QColor("BLUE"));
            return ;
        }
    }
    qDebug() << "send ok";
}

void ChatForm::receiveSuccess(ChatMessage message){
    QListWidgetItem *mesItem = new QListWidgetItem(ui->chatListWidget);
    mesItem->setData(Qt::UserRole, message.getUuid().toString());
    mesItem->setText(QString("receive:\r\n").append(message.getContent()));
    ui->chatListWidget->setCurrentItem(mesItem);
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
    ChatMessage message(ChatMessage::Request, receiver.getUuid(), ui->messagePlainTextEdit->toPlainText(), this);
    mChatService->send(message, receiver.getIp());
    QListWidgetItem *mesItem = new QListWidgetItem(ui->chatListWidget);
    mesItem->setData(Qt::UserRole, message.getUuid().toString());
    mesItem->setText(QString("%1:\r\n").arg(SettingUtil::getUtil()->getSender()->getName()).append(message.getContent()));
    ui->messagePlainTextEdit->clear();
}
