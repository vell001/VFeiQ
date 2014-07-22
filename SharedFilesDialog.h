#ifndef SHAREDFILESDIALOG_H
#define SHAREDFILESDIALOG_H

#include <QDialog>
#include "model/User.h"
#include "model/FileMessage.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHash>
#include "service/FileShareService.h"
#include <QCheckBox>
#include "QProgressBar"
#include "model/FileReceiver.h"
#include <QMessageBox>
#include <QFileDialog>
#include "service/UserService.h"
#include <QDesktopServices>

namespace Ui {
class SharedFilesDialog;
}

class SharedFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SharedFilesDialog(User *sharer, QWidget *parent = 0);
    ~SharedFilesDialog();
private slots:
    void receivedSharedFiles(QHostAddress,quint16,ChatMessage);
    void on_selectAllButton_clicked();

    void on_cancelSelectButton_clicked();

    void on_refreshButton_clicked();

    void on_cancelButton_clicked();

    void on_downloadButton_clicked();

    void fileReceiveError(QString error);
    void fileReceiveProgress(qint64 maximum, qint64 value);
    void fileReceiveEnd(qint64 totalBytes);

private:
    Ui::SharedFilesDialog *ui;
    User *mSharer;
    User *myself;

    QStandardItemModel *sharedFilesModel;
    QHash<QUuid, FileMessage *> *mFileMessages;
    void updateFilesView();

    FileShareService *mFileShareService;
    FileReceiver *mFileReceiver;
    FileMessage *mFileMessage;

    QModelIndexList selectedList;
    int fileReceiveRow;
    QString saveDirName;
    void getSelectedRows();

};
#endif // SHAREDFILESDIALOG_H
