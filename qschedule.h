#ifndef QSCHEDULE_H
#define QSCHEDULE_H
#include<QTime>
#include<QDate>
#include<QDateTime>
#include <QMainWindow>
#include<QSystemTrayIcon>
#include <QCloseEvent>
#include <QWidget>
#include "allevents.h"
#include"addevt.h"
#include"Record.h"
#include "deletevent.h"
namespace Ui {
class Qschedule;
}

class Qschedule : public QWidget
{
    Q_OBJECT

public:
    explicit Qschedule(QWidget *parent = 0);
    ~Qschedule();
    Addevt* addevt;
    AllEvents* tableform;
    DeletEvent* deletmap;
    void todayshow(vector<Record> RList);

private:
    Ui::Qschedule *ui;


private slots:
    //void timerUpdate();
    //void on_pAdd_clicked();
    //void on_todayBtn_clicked();
    void on_allBtn_clicked();
    void on_quitBtn_clicked();

    //void on_calendarWidget_clicked(const QDate &date);
   //void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void on_calendarWidget_activated(const QDate &date);
    void changeColor();
    void on_pDlt_clicked();
};

#endif // QSCHEDULE_H
