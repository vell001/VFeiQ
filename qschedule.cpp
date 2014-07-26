#include "qschedule.h"
#include "ui_qschedule.h"
#include <QDateTime>
#include <QTimer>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSound>
#include <QApplication>
#include "QString"
#include <QtGui>
#include "addevt.h"
#include <QTextCharFormat>

Qschedule::Qschedule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Qschedule)
{
    //qDebug(__func__);
    ui->setupUi(this);
    this->setWindowTitle(tr("日程安排提醒"));
    addevt = new Addevt();
    tableform = new AllEvents();
    addevt->load(ui->calendarWidget);
    ui->calendarWidget->show();
    (addevt,SIGNAL(RecordSaved()),this,SLOT(changeColor()));
    this->setStyleSheet("QWidget#Qschedule{background-color:#ffdbf8;}");
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss dddd");
    QString current_time = time;
    ui->timeTextBrowser->append(current_time );

    todayshow(addevt->RList);
    deletmap = new DeletEvent(addevt->RList);
}

Qschedule::~Qschedule()
{
    delete ui;
}


void Qschedule::on_allBtn_clicked()
{
    qDebug(__func__);
    tableform->show();
    tableform->shows(addevt->RList);

}

void Qschedule::on_quitBtn_clicked()
{
       this-> close();
}


void Qschedule::on_calendarWidget_activated(const QDate &date)
{
    QDate t = ui->calendarWidget->selectedDate();
    addevt->setDate(t.toString());
    addevt->apperRecord();
    addevt->exec();
    todayshow(addevt->RList);
}

void Qschedule::changeColor()
{
    QDate t = ui->calendarWidget->selectedDate();
    QTextCharFormat chooseFormat;
    chooseFormat.setForeground(Qt::blue);
    ui->calendarWidget->setDateTextFormat(t ,chooseFormat);

}


void Qschedule::todayshow(vector<Record> RList)
{
    qDebug(__func__);
    QString today = QDate::currentDate().toString();
    QString m = "今天没有备忘事件";
    qDebug()<<today;
    for(vector<Record>::iterator it=RList.begin();it!=RList.end();it++)
    {
        qDebug()<<it->Date;
        if(it->Date == today)
        {
            qDebug()<<it->record;
            ui->eventTextBrowser->clear();
            ui->eventTextBrowser->append(it->record);
        }
    }
}

void Qschedule::on_pDlt_clicked()
{

    deletmap->show();
}
