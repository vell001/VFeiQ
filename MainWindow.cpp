#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon icon = QIcon(QString(":/images/user_image.png"));

    ui->userImage->setIcon(icon);
    ui->userImage->setIconSize(QSize(50, 50));

    ui->weatherLabel->setText(QString("<img src=\":/images/weather_1.gif\"/>"));

    ui->searchLabel->setText(QString("<img src=\":/images/search.png\" width='24' height='24'/>"));

    ui->signatureLabel->setText(QString("signatrue"));

    for(int i=0; i<5; i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->contentsTreeWidget);
        item->setText(0,QString("number: %1").arg(i+1));
        for(int j=0; j<4 && i <3; j++) {
            QTreeWidgetItem *cItem = new QTreeWidgetItem(item);
            cItem->setText(0,QString("child"));
            cItem->setText(1,QString::number(j));
            cItem->setIcon(0, icon);
        }
    }
    ui->contentsTreeWidget->setIconSize(QSize(40, 40));
    connect(ui->contentsTreeWidget,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openChatForm(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openChatForm(QModelIndex index){
    qDebug() << "openChatForm " << index.row();
    qDebug() << ui->contentsTreeWidget->currentItem()->data(1,0).toString();
    if(ui->contentsTreeWidget->currentItem()->childCount() == 0){
//        (new ChatForm())->show();
    }
}

