#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QShowEvent>
#include <QIcon>
#include <QDesktopWidget>
#include <QDebug>
#include <QTime>
#include <QHash>
#include <QUuid>

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MessageDialog(QWidget *parent = 0);
    explicit MessageDialog(const QString & title, const QString & message, const QIcon &icon = QIcon(),
                           int millisecondsTimeoutHint = 3000, const QUuid &uuid = QUuid::createUuid(), QWidget *parent = 0);

    ~MessageDialog();
    QUuid getUuid();
    void setUuid(const QUuid &uuid);
protected:
    virtual void showEvent ( QShowEvent * event );
//    virtual void closeEvent ( QCloseEvent * e );
signals:
    void accepted(QUuid);
    void rejected(QUuid);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::MessageDialog *ui;
    QTimer mTimer;
    int millisecondsTimeoutHint;
    QUuid uuid;

    void setPosition();
};

#endif // MESSAGEDIALOG_H
