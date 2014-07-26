#include "WeatherDialog.h"
#include "ui_WeatherDialog.h"

WeatherDialog::WeatherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeatherDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->webView->setHtml("<iframe allowtransparency=\"true\" frameborder=\"0\" width=\"565\" height=\"98\" scrolling=\"no\" src=\"http://tianqi.2345.com/plugin/widget/index.htm?s=2&z=3&t=1&v=0&d=3&bd=0&k=&f=&q=1&e=1&a=1&c=54511&w=565&h=98&align=center\"></iframe>");
    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

WeatherDialog::~WeatherDialog()
{
    delete ui;
}

void WeatherDialog::on_webView_linkClicked(const QUrl &arg1)
{
    QDesktopServices::openUrl(arg1);
}

void WeatherDialog::on_okButton_clicked()
{
    close();
}
