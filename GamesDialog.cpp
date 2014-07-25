#include "GamesDialog.h"
#include "ui_GamesDialog.h"

GamesDialog::GamesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GamesDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->webView->setUrl(QUrl("http://www.3366.com/"));
//    ui->webView->setUrl(QUrl("file:///D:/Qt/workspace/build-VFeiQ-MinGW-Debug/debug/wordShow/index.html"));
    ui->webView->setMouseTracking(true);
    ui->webView->setGeometry(this->geometry());
    ui->webView->page()->settings()->setAttribute(QWebSettings::JavaEnabled, true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::LocalStorageDatabaseEnabled, true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    ui->webView->page()->settings()->setAttribute(QWebSettings::AutoLoadImages, true);
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
