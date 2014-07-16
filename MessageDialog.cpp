#include "MessageDialog.h"
#include "ui_MessageDialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog),
    mTimer(this),
    uuid(QUuid::createUuid())
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(close()));
}

MessageDialog::MessageDialog(const QString & title, const QString & message, const QIcon &icon,
                             int millisecondsTimeoutHint, const QUuid &uuid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog),
    mTimer(this),
    millisecondsTimeoutHint(millisecondsTimeoutHint),
    uuid(uuid)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);
    setWindowTitle(title);
    ui->textBrowser->setHtml(message);
    setWindowIcon(icon);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(close()));
}

void MessageDialog::setPosition(){
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //得到客户区矩形
    QRect clientRect = desktopWidget->availableGeometry();
    QRect dialogRect = geometry();
    setGeometry(clientRect.width()-dialogRect.width()-5, clientRect.height()-dialogRect.height()-5, dialogRect.width(), dialogRect.height());
}

void MessageDialog::showEvent ( QShowEvent * event ){
    event->accept();
    setPosition();
    if(millisecondsTimeoutHint > 0){
        mTimer.start(millisecondsTimeoutHint);
    }
}

//void MessageDialog::closeEvent ( QCloseEvent * e ){
//    e->accept();
//}

QUuid MessageDialog::getUuid(){
    return uuid;
}

void MessageDialog::setUuid(const QUuid &uuid){
    this->uuid = uuid;
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::on_buttonBox_accepted()
{
    emit accepted(uuid);
    this->close();
}

void MessageDialog::on_buttonBox_rejected()
{
    emit rejected(uuid);
    this->close();
}
