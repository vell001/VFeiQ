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
#include "service/FileMessageService.h"
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
#include <QPair>
#include <QStandardItemModel>
#include <QProgressBar>
#include <QMessageBox>
#include "model/FileSender.h"
#include "model/FileReceiver.h"
#include "model/FileMessage.h"
#include "MessageDialog.h"
#include <QDesktopServices>
#include "SharedFilesDialog.h"
#include "ScreenshotsWidget.h"
#include "FacesDialog.h"

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

    void shakeTimeOut();

    void facesClicked(Image image);

    void screenshotsFinish(QPixmap);

    void on_closeButton_clicked();

    void on_fontButton_clicked();

    void on_colorButton_clicked();

    void on_facesButton_clicked();

    void on_choseFileButton_clicked();

    void on_cancelSendButton_clicked();

    void on_sendFileButton_clicked();

    void on_saveReceiveButton_clicked();

    void on_cancelReceiveButton_clicked();

    void on_chatRecordBrowser_anchorClicked(const QUrl &arg1);

    void on_shakeButton_clicked();

    void on_sharedFilesButton_clicked();

    void on_grapScreenButton_clicked();

    void on_sendImageButton_clicked();

signals:
    void closed(QUuid receiverUuid);
private:
    Ui::ChatForm *ui;
    ChatService *mChatService;
    User *sender;
    User *receiver;
    void sendMessage();

    QList<ChatRecord *> mChatRecords;
    void updateChatRecordView();

    IconService *mIconService;

    void loadSetting();
    void saveSetting();

    QStandardItemModel *sendFileModel;
    QStandardItemModel *receiveFileModel;

    FileSender *mFileSender;
    FileReceiver *mFileReceiver;
    FileMessage *mFileMessage;
    FileMessageService *mFileMsgService;
    int fileSendRow;
    int fileReceiveRow;
    QString saveDirName;

    void clearSendFiles();
    void clearReceiveFiles();

    // shake
    QTimer* mShakeTimer;
    int mShakePosition;
    QPoint mShakeCurPos;
    int shakeMaxLimitTimes;
    int shakeMaxLimitSpace;
    void startShake();

    // screen shot
    ScreenshotsWidget *mScreenshotsWidget;

    // faces
    FacesDialog mFacesDialog;
};

#endif // CHATFORM_H
