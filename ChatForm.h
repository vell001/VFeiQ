/**********************************************************************
** Copyright (C) 2014 vell001
** Author: VellBibi
** Description:
**
**********************************************************************/
#ifndef CHATFORM_H
#define CHATFORM_H

#include <QWidget>
#include "service/ChatService.h"
#include "service/BroadcastService.h"
#include <QListWidgetItem>
#include <QKeyEvent>
#include <model/User.h>
#include <QList>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QSettings>
#include "service/ChatRecordService.h"
#include "service/IconService.h"
#include <QStringList>
#include <QFile>
#include <QHash>
#include <QStandardItemModel>
#include <QProgressBar>
#include <QMessageBox>
#include "model/FileSender.h"
#include "model/FileReceiver.h"
#include "model/FileMessage.h"
#include "MessageDialog.h"

namespace Ui {
class ChatForm;
}

class ChatForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatForm(User *receiver, QWidget *parent = 0);
    ~ChatForm();

protected:
    void keyPressEvent(QKeyEvent *e);
    virtual void closeEvent ( QCloseEvent * event );
private slots:
    void on_sendButton_clicked();
    void sendError(QUuid messageUuid, QString errorMessage);
    void sendSuccess(QUuid messageUuid);
    void receiveSuccess(QHostAddress senderIp, quint16 senderPort, ChatMessage message);

    void fileMsgReceived(QHostAddress senderIp, quint16 senderPort, ChatMessage message);

    void fileSendEnd(qint64);
    void fileSendError(QString);
    void fileSendProgress(qint64,qint64);

    void fileReceiveEnd(qint64);
    void fileReceiveError(QString);
    void fileReceiveProgress(qint64,qint64);

    void on_closeButton_clicked();

    void on_fontButton_clicked();

    void on_colorButton_clicked();

    void on_facesButton_clicked();

    void on_choseFileButton_clicked();

    void on_cancelSendButton_clicked();

    void on_sendFileButton_clicked();

signals:
    void closed(QUuid receiverUuid);
private:
    Ui::ChatForm *ui;
    ChatService *mChatService;
    User *sender;
    User *receiver;
    void sendMessage();

    QList<ChatRecord *> mChatRecords;
    void initForm();
    void updateChatRecordView();

    IconService *mIconService;

    void loadSetting();
    void saveSetting();

    QStandardItemModel *sendFileModel;

    FileSender *mFileSender;
    FileReceiver *mFileReceiver;
    FileMessage *mFileMessage;
    BroadcastService *mFileMsgService;
    int fileSendRow;
    void clearSendFiles();
};

#endif // CHATFORM_H
