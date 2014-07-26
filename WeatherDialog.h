#ifndef WEATHERDIALOG_H
#define WEATHERDIALOG_H

#include <QDialog>

namespace Ui {
class WeatherDialog;
}

class WeatherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WeatherDialog(QWidget *parent = 0);
    ~WeatherDialog();

private:
    Ui::WeatherDialog *ui;
};

#endif // WEATHERDIALOG_H
