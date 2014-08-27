#ifndef WEATHERDIALOG_H
#define WEATHERDIALOG_H

#include <QDialog>
#include <QDesktopServices>

namespace Ui {
class WeatherDialog;
}

class WeatherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WeatherDialog(QWidget *parent = 0);
    ~WeatherDialog();

private slots:
    void on_webView_linkClicked(const QUrl &arg1);

    void on_okButton_clicked();

private:
    Ui::WeatherDialog *ui;
};

#endif // WEATHERDIALOG_H
