#ifndef SETSHAREFILESDIALOG_H
#define SETSHAREFILESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHash>
#include <QCheckBox>
#include "service/StorageService.h"
#include <QFileDialog>

namespace Ui {
class SetShareFilesDialog;
}

class SetShareFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetShareFilesDialog(QWidget *parent = 0);
    ~SetShareFilesDialog();

private slots:

    void on_confirmButtonBox_accepted();

    void on_confirmButtonBox_rejected();

    void on_addFileButton_clicked();

    void on_deleteButton_clicked();

    void on_selectAllButton_clicked();

    void on_cancelSelectButton_clicked();

private:
    Ui::SetShareFilesDialog *ui;
    StorageService *mStorageService;

    QStandardItemModel *sharedFilesModel;
    QHash<QUuid, FileMessage *> *mFileMessages;

    void loadFileMessages();

    void updateFilesView();
};

#endif // SETSHAREFILESDIALOG_H
