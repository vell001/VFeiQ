#include "deletevent.h"
#include "ui_deletevent.h"
#include "qmessagebox.h"
#include "QDate"


DeletEvent::DeletEvent(vector<Record> &RList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeletEvent)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("删除备忘"));
    this->setStyleSheet("DeletEvent#DeletEvent{background-color:#ffdbf8;}");
    this->RList = &RList;
}

DeletEvent::~DeletEvent()
{
    delete ui;
}

void DeletEvent::on_tureBtn_clicked()
{
    dlt();
    this->close();
}

void DeletEvent::on_cancleBtn_clicked()
{
    this->close();
}

void DeletEvent::dlt()
{
    qDebug(__func__);
    QString thedate=ui->dateEdit->date().toString();
    QString today = QDate::currentDate().toString();
    qDebug()<<thedate;
    for(vector<Record>::iterator it=RList->begin();it!=RList->end();it++)
    {

        if(it->Date==thedate)
        {
            RList->erase(it);
        }
    }

}
