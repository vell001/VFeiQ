#include "ChatForm.h"
#include "ui_ChatForm.h"

ChatForm::ChatForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatForm)
{
    ui->setupUi(this);
    mChatUDPSocket = new ChatUDPSocket(this);
    connect(mChatUDPSocket, SIGNAL(receiveSuccess(QString)), this, SLOT(receiveSuccess(QString)));
    connect(mChatUDPSocket, SIGNAL(sendError(QString)), this, SLOT(sendError(QString)));
    connect(mChatUDPSocket, SIGNAL(sendSuccess(QUuid)), this, SLOT(sendSuccess(QUuid)));
}

ChatForm::~ChatForm()
{
    delete ui;
    delete mChatUDPSocket;
}

void ChatForm::on_sendButton_clicked()
{
    // send message;
    sendMessage();
}

void ChatForm::sendError(QString errorMessage){

}

void ChatForm::sendSuccess(QUuid messageUuid){

}

void ChatForm::receiveSuccess(QString content){
    QListWidgetItem *mesItem = new QListWidgetItem(ui->chatListWidget);
    mesItem->setText(QString("receive:\r\n").append(content));
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
    ChatMessage message;
    message.setContent(ui->messagePlainTextEdit->toPlainText());
    message.setReceiver(QHostAddress("222.24.75.99"));
    mChatUDPSocket->send(&message);
    QListWidgetItem *mesItem = new QListWidgetItem(ui->chatListWidget);
    mesItem->setText(QString("send:\r\n").append(message.getContent()));
    ui->messagePlainTextEdit->clear();
}
