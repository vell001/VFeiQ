#ifndef ADDEVT_H
#define ADDEVT_H

#include <QDialog>
#include"Record.h"
#include <vector>
#include <QCalendarWidget>
#include <QFile>
#include <QTextCharFormat>
#include <QTextStream>
#include <QDate>
#include <QDebug>
using namespace std;

namespace Ui {
class Addevt;
}
//vector<Record> RList;
class Addevt : public QDialog
{
    Q_OBJECT

public:
    explicit Addevt(QWidget *parent = 0);
    ~Addevt();
    QString Sdate;
    Record rd;
    void setDate(QString date);
    void apperRecord();
    void load(QCalendarWidget* ca);
    void saveFile(QString fileName);
    vector<Record> RList;
private slots:

    void on_turnBtn_clicked();

    void on_canclBtn_clicked();

signals:
    void RecordSaved();
private:
    Ui::Addevt *ui;
};

#endif // ADDEVT_H
