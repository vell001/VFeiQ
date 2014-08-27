#include "FacesDialog.h"
#include "ui_FacesDialog.h"

FacesDialog::FacesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FacesDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    mFacesDirs << QFileInfo(":/faces001") << QFileInfo(":/faces002") << QFileInfo(":/faces003");
    mFacesTableWidgets << ui->facesTableWidget001
                       << ui->facesTableWidget002
                       << ui->facesTableWidget003;
    foreach (QTableWidget * widget, mFacesTableWidgets) {
        widget->setIconSize(QSize(40, 40));
    }
    initFacesWidget();
}

FacesDialog::~FacesDialog()
{
    delete ui;
}

void FacesDialog::initFacesWidget(){
    QStringList nameFilters;
    nameFilters << "*.png" << "*.gif" << "*.jpg";

    for(int i=0; i<3; i++) {
        QFileInfo facesDirInfo  = mFacesDirs[i];
        if(!facesDirInfo.exists()) continue;

        QTableWidget * widget = mFacesTableWidgets[i];
        QAbstractItemModel *model = widget->model();

        QDir facesDir(facesDirInfo.filePath());
        QFileInfoList faceFiles = facesDir.entryInfoList(nameFilters);

        bool flag = true;
        widget->setColumnCount(10);
        widget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        widget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        int maxRow = faceFiles.count()%10 == 0 ? faceFiles.count() / 10 : (faceFiles.count() / 10) + 1;
        for(int r=0; r<maxRow; r++) {
            widget->insertRow(r);
            for(int c=0; c<10; c++) {
                if(r*10+c < faceFiles.count()) {
                    QFileInfo faceFile = faceFiles[r*10+c];
                    QTableWidgetItem *item = new QTableWidgetItem;
                    widget->setItem(r, c, item);
                    item->setIcon(QIcon(faceFile.filePath()));
                    item->setData(Qt::UserRole, faceFile.filePath()); // user data save file path
                } else {
                    flag = false;
                }
            }
        }
    }
}

void FacesDialog::on_facesTableWidget001_clicked(const QModelIndex &index)
{
    emit clicked(Image(QImage(index.data(Qt::UserRole).toString())));
    hide();
}

void FacesDialog::on_facesTableWidget002_clicked(const QModelIndex &index)
{
    emit clicked(Image(QImage(index.data(Qt::UserRole).toString())));
    hide();
}

void FacesDialog::on_facesTableWidget003_clicked(const QModelIndex &index)
{
    emit clicked(Image(QImage(index.data(Qt::UserRole).toString())));
    hide();
}
