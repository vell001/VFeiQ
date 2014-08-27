#include "SetShareFilesDialog.h"
#include "ui_SetShareFilesDialog.h"

SetShareFilesDialog::SetShareFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetShareFilesDialog),
    mStorageService(StorageService::getService()),
    sharedFilesModel(new QStandardItemModel)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    mFileMessages = 0;

    QStringList heads;
    heads << "filename" << "size" << "type" << "url" << "uuid";
    sharedFilesModel->setHorizontalHeaderLabels(heads);
    ui->FilesTableView->setModel(sharedFilesModel);
    ui->FilesTableView->setSelectionBehavior(QTableView::SelectRows);

    loadFileMessages();
}

SetShareFilesDialog::~SetShareFilesDialog()
{
    delete ui;
}

void SetShareFilesDialog::loadFileMessages(){
    mFileMessages = mStorageService->getSharedFilesMessages();
    updateFilesView();
}

void SetShareFilesDialog::updateFilesView(){
    sharedFilesModel->removeRows(0, sharedFilesModel->rowCount()); // clear exist data
//    heads << "filename" << "size" << "type" << "url" << "uuid";
    foreach (FileMessage *fileMsg, mFileMessages->values()) {
        QList<QStandardItem *> items;
        items << new QStandardItem(fileMsg->getFileName())
             << new QStandardItem(QString::number(fileMsg->getSize()))
             << new QStandardItem(fileMsg->getType()==FileMessage::File ? "file" : "dir")
             << new QStandardItem(fileMsg->getFileFullName())
             << new QStandardItem(fileMsg->getUuid().toString());
        items[0]->setData(fileMsg->getUuid().toString(), Qt::UserRole);
        sharedFilesModel->appendRow(items);
    }
}

void SetShareFilesDialog::on_confirmButtonBox_accepted() {
    mStorageService->storeSharedFilesMessages(mFileMessages);
}

void SetShareFilesDialog::on_confirmButtonBox_rejected() {
    close();
}

void SetShareFilesDialog::on_addFileButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this);
    foreach (QString fileName, fileNames) {
        FileMessage *fileMsg = new FileMessage(QFileInfo(fileName));
        (*mFileMessages)[fileMsg->getUuid()] = fileMsg;
    }
    updateFilesView();
}

void SetShareFilesDialog::on_deleteButton_clicked()
{
    QItemSelectionModel *selectionModel = ui->FilesTableView->selectionModel();
    QModelIndexList selected = selectionModel->selectedRows(0);
    foreach (QModelIndex index, selected) {
        QUuid key(index.data(Qt::UserRole).toString());
        mFileMessages->remove(key);
    }
    updateFilesView();
}

void SetShareFilesDialog::on_selectAllButton_clicked()
{
    ui->FilesTableView->selectAll();
}

void SetShareFilesDialog::on_cancelSelectButton_clicked()
{
    ui->FilesTableView->clearSelection();
}
