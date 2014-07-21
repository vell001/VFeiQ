#include "SharedFilesDialog.h"
#include "ui_SharedFilesDialog.h"

SharedFilesDialog::SharedFilesDialog(User *sharer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SharedFilesDialog),
    mSharer(sharer)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    sharedFilesModel = new QStandardItemModel;
    mFileMessages = 0;

    QStringList heads;
    heads << "" << "filename" << "size" << "type" << "url" << "uuid";
    sharedFilesModel->setHorizontalHeaderLabels(heads);
    ui->FilesTableView->setModel(sharedFilesModel);
}

SharedFilesDialog::~SharedFilesDialog()
{
    delete ui;
}

void SharedFilesDialog::updateFilesView(){

}

void SharedFilesDialog::updateFileMessages(){

}
