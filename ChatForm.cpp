#include "ChatForm.h"
#include "ui_ChatForm.h"

ChatForm::ChatForm(User *receiver, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatForm),
    receiver(receiver),
    sender(UserService::getService()->getMyself()),
    mChatRecords(ChatRecordService::getService()->getChatRecordsByUserUuid(receiver->getUuid(), ChatRecord::NotRead))
{
    initForm();
    foreach (ChatRecord *record, mChatRecords) {
        QListWidgetItem *mesItem = new QListWidgetItem(ui->chatListWidget);
        mesItem->setData(Qt::UserRole, record->getUuid().toString());
        mesItem->setText(QString("%1:\r\n%2").arg(receiver->getName()).arg(QString(record->getContent())));
        mesItem->setBackgroundColor(QColor("#F0E68C"));
        mesItem->setIcon(mIconService->getIconByUuid(receiver->getIconUuid()));
        ui->chatListWidget->setCurrentItem(mesItem);
    }
}

void ChatForm::initForm(){
    ui->setupUi(this);
    this->mChatService = ChatService::getService();

    ui->userIcon->setIcon(mIconService->getIconByUuid(this->receiver->getIconUuid()));
    ui->userIcon->setIconSize(QSize(40, 40));

    ui->usernameLabel->setText(this->receiver->getName());
    ui->signatrueLabel->setText(this->receiver->getInfo());

    ui->chatListWidget->setIconSize(QSize(40,40));

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
    QListWidgetItem *mesItem;
    for(int i=ui->chatListWidget->count()-1; i>=0; i--) {
        mesItem = ui->chatListWidget->item(i);
        if(messageUuid.toString() == mesItem->data(Qt::UserRole).toString()) {
            mesItem->setBackgroundColor(QColor("#FFF68F"));
            return ;
        }
    }
    qDebug() << "send ok";
}

void ChatForm::receiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    qDebug() << message.getSenderUuid() << receiver->getUuid();
    if(message.getSenderUuid() == receiver->getUuid()) {
        QListWidgetItem *mesItem = new QListWidgetItem(ui->chatListWidget);
        mesItem->setData(Qt::UserRole, message.getUuid().toString());
        mesItem->setText(QString("%1:\r\n%2").arg(receiver->getName()).arg(QString(message.getContent())));
        mesItem->setBackgroundColor(QColor("#F0E68C"));
        mesItem->setIcon(mIconService->getIconByUuid(receiver->getIconUuid()));
        ui->chatListWidget->setCurrentItem(mesItem);
        qDebug() << "receiveSuccess: " << senderIp << senderPort;
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
    ChatMessage message(ChatMessage::Request, sender->getUuid(), ui->messagePlainTextEdit->toPlainText(), this);
    mChatService->send(message, receiver->getIp());
    QListWidgetItem *mesItem = new QListWidgetItem(ui->chatListWidget);
    mesItem->setData(Qt::UserRole, message.getUuid().toString());
    mesItem->setText(QString("%1:\r\n").arg(sender->getName()).append(message.getContent()));
    mesItem->setIcon(mIconService->getIconByUuid(sender->getIconUuid()));
    ui->chatListWidget->setCurrentItem(mesItem);
    ui->messagePlainTextEdit->clear();
}
