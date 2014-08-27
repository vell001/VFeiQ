#ifndef FACESDIALOG_H
#define FACESDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include "model/Image.h"
#include <QList>
#include <QDir>
#include <QAbstractItemModel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>

namespace Ui {
class FacesDialog;
}

class FacesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FacesDialog(QWidget *parent = 0);
    ~FacesDialog();
signals:
    void clicked(Image);
private slots:
    void on_facesTableWidget001_clicked(const QModelIndex &index);

    void on_facesTableWidget002_clicked(const QModelIndex &index);

    void on_facesTableWidget003_clicked(const QModelIndex &index);

private:
    Ui::FacesDialog *ui;
    void initFacesWidget();
    QList<QFileInfo> mFacesDirs;
    QList<QTableWidget *> mFacesTableWidgets;
};

#endif // FACESDIALOG_H
