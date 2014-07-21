#ifndef SHAREDFILESDIALOG_H
#define SHAREDFILESDIALOG_H

#include <QDialog>
#include "model/User.h"
#include "model/FileMessage.h"
#include <QStandardItemModel>
#include <QHash>

namespace Ui {
class SharedFilesDialog;
}

class SharedFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SharedFilesDialog(User *sharer, QWidget *parent = 0);
    ~SharedFilesDialog();

private:
    Ui::SharedFilesDialog *ui;
    User *mSharer;

    QStandardItemModel *sharedFilesModel;
    QHash<QUuid, FileMessage> *mFileMessages;
    void updateFilesView();
    void updateFileMessages();

    void loadFileMessages();
    void saveFileMessages();
};
#endif // SHAREDFILESDIALOG_H
