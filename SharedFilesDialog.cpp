#include "SharedFilesDialog.h"
#include "ui_SharedFilesDialog.h"

SharedFilesDialog::SharedFilesDialog(User *sharer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SharedFilesDialog),
    mSharer(sharer),
    myself(UserService::getService()->getMyself()),
    mFileShareService(FileShareService::getService()),
    sharedFilesModel(new QStandardItemModel),
    mFileMessage(0),
    mFileReceiver(0),
    mFileMessages(0)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QStringList heads;
    heads << "filename" << "size" << "type" << "url" << "uuid" << "progress";
    sharedFilesModel->setHorizontalHeaderLabels(heads);
    ui->FilesTableView->setModel(sharedFilesModel);
    ui->FilesTableView->setSelectionBehavior(QTableView::SelectRows);

    connect(mFileShareService, SIGNAL(receivedSharedFiles(QHostAddress,quint16,ChatMessage)), this, SLOT(receivedSharedFiles(QHostAddress,quint16,ChatMessage)));

    mFileShareService->sendSharedFilesRequest(sharer->getIp());
}

SharedFilesDialog::~SharedFilesDialog()
{
    delete ui;
}

void SharedFilesDialog::receivedSharedFiles(QHostAddress senderIp, quint16 senderPort, ChatMessage message){
    QString sharedFilesXML = message.getContent();
    delete mFileMessages;
    mFileMessages = FileMessage::parseFileMessages(sharedFilesXML);
    updateFilesView();
}

void SharedFilesDialog::updateFilesView(){
    sharedFilesModel->removeRows(0, sharedFilesModel->rowCount()); // clear exist data
//    heads << "filename" << "size" << "type" << "url" << "uuid";
    foreach (FileMessage *fileMsg, mFileMessages->values()) {
        QList<QStandardItem *> items;
        items << new QStandardItem(fileMsg->getFileName())
             << new QStandardItem(QString::number(fileMsg->getSize()))
             << new QStandardItem(fileMsg->getType()==FileMessage::File ? "file" : "dir")
             << new QStandardItem(fileMsg->getFileFullName())
             << new QStandardItem(fileMsg->getUuid().toString())
             << new QStandardItem;
        items[0]->setData(fileMsg->getUuid().toString(), Qt::UserRole);
        sharedFilesModel->appendRow(items);
    }
    for(int i=0; i<sharedFilesModel->rowCount(); i++) {
        QModelIndex index = sharedFilesModel->index(i, 5);
        ui->FilesTableView->setIndexWidget(index, new QProgressBar);
    }
}

void SharedFilesDialog::fileReceiveError(QString error){
    QMessageBox::warning(this, "file receive error", "file receive error: \r\n" + error);
}

void SharedFilesDialog::fileReceiveProgress(qint64 maximum, qint64 value){
    QModelIndex index = sharedFilesModel->index(selectedList[fileReceiveRow].row(), 5);
    QProgressBar *bar = (QProgressBar *)ui->FilesTableView->indexWidget(index);
    bar->setMaximum(maximum);
    bar->setValue(value);
}

void SharedFilesDialog::fileReceiveEnd(qint64 totalBytes){
    if(fileReceiveRow+1 < selectedList.size()) {
        fileReceiveRow ++;
        delete mFileMessage;
        QUuid fUuid(QUuid(selectedList[fileReceiveRow].data(Qt::UserRole).toString()));
        mFileMessage = new FileMessage(QFileInfo((*mFileMessages)[fUuid]->getFileFullName()), fUuid);

        // start receiver
        delete mFileReceiver;
        mFileMessage->setTransferPort(1214);
        mFileReceiver = new FileReceiver(new QFile(saveDirName+"/"+mFileMessage->getFileName()),
                                         QHostAddress::Any,
                                         mFileMessage->getTransferPort(),
                                         mFileMessage->getUuid());
        connect(mFileReceiver, SIGNAL(receiveEnd(qint64)), this, SLOT(fileReceiveEnd(qint64)));
        connect(mFileReceiver, SIGNAL(receiveError(QString)), this, SLOT(fileReceiveError(QString)));
        connect(mFileReceiver, SIGNAL(receiveProgress(qint64,qint64)), this, SLOT(fileReceiveProgress(qint64,qint64)));

        mFileReceiver->start();

        ChatMessage fileMsg(fUuid, ChatMessage::Request, myself->getUuid(), mFileMessage->toString(), ChatMessage::FileXML);
        mFileShareService->send(fileMsg, mSharer->getIp());
    } else {
        if(QMessageBox::question(this, "download success",
                                 "download success\r\n do you want to open save directory",
                                 QMessageBox::Ok, QMessageBox::Cancel)
                == QMessageBox::Ok) {
            QDesktopServices::openUrl(QUrl("file:///" + saveDirName, QUrl::TolerantMode));
        }
        close();
    }
}

void SharedFilesDialog::on_downloadButton_clicked()
{
    saveDirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    saveDirName,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(saveDirName.isEmpty()){
        QMessageBox::warning(this, "directory error", "directory is not exist");
        return;
    } else { // start receive
        QItemSelectionModel *selectionModel = ui->FilesTableView->selectionModel();
        selectedList = selectionModel->selectedRows(0);
        fileReceiveRow = -1;
        fileReceiveEnd(0);
    }
}

void SharedFilesDialog::on_selectAllButton_clicked()
{
    ui->FilesTableView->selectAll();
}

void SharedFilesDialog::on_cancelSelectButton_clicked()
{
    ui->FilesTableView->clearSelection();
}

void SharedFilesDialog::on_refreshButton_clicked()
{
    mFileShareService->sendSharedFilesRequest(mSharer->getIp());
}

void SharedFilesDialog::on_cancelButton_clicked()
{
    close();
}
