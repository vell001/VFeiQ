#include "GamesDialog.h"
#include "ui_GamesDialog.h"

GamesDialog::GamesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GamesDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->webView->setUrl(QUrl("http://www.3366.com/"));
    ui->webView->setMouseTracking(true);
    ui->webView->setGeometry(this->geometry());

    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

GamesDialog::~GamesDialog()
{
    delete ui;
}

void GamesDialog::on_webView_linkClicked(const QUrl &arg1)
{
    QDesktopServices::openUrl(arg1);
}
