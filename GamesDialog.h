#ifndef GAMESDIALOG_H
#define GAMESDIALOG_H

#include <QDialog>
#include <QUrl>
#include <QDesktopServices>

namespace Ui {
class GamesDialog;
}

class GamesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GamesDialog(QWidget *parent = 0);
    ~GamesDialog();

private slots:
    void on_webView_linkClicked(const QUrl &arg1);

private:
    Ui::GamesDialog *ui;
};

#endif // GAMESDIALOG_H
