#include "allevents.h"
#include "ui_allevents.h"
#include "QStringList"
#include "addevt.h"
#include "QTextStream"
#include "QMessageBox"

AllEvents::AllEvents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllEvents)
{
    qDebug(__func__);
    ui->setupUi(this);
    this->setStyleSheet("QWidget#AllEvents{background-color:#ffdbf8;}");
    this->setWindowTitle(tr("备忘本"));
}

AllEvents::~AllEvents()
{
    delete ui;
}

void AllEvents::on_closeBtn_clicked()
{
    this-> close();
}

void AllEvents::shows(vector<Record> RList)
{
     qDebug(__func__);
     QString n="\n";
     qDebug()<<RList.size();
     ui->textBrowser->clear();
     for(vector<Record>::iterator it=RList.begin();it!=RList.end();it++){
         ui->textBrowser->append(n);
         QString d,r;
         d=it->Date;
         r=it->record;
         ui->textBrowser->append(d);
         ui->textBrowser->append(r);
     }

}
