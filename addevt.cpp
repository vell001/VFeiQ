#include "addevt.h"
#include "ui_addevt.h"

Addevt::Addevt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Addevt)
{
    qDebug(__func__);
    ui->setupUi(this);
    this->setWindowTitle(tr("添加备忘"));
    this->setStyleSheet("QDialog#Addevt{background-color:#ffdbf8;}");
    QString now= QDateTime::currentDateTime().toString(" yyyy-MM-dd hh:mm:ss dddd");
    QString current_time = now;
    ui->textBrowser->append(current_time );
    ui->textEdit->clear();
}

Addevt::~Addevt()
{
    delete ui;
}

void Addevt::setDate(QString date)
{
    qDebug(__func__);
    this->Sdate=date;
    qDebug()<<Sdate;
}

void Addevt::apperRecord()
{
    qDebug(__func__);
    for(vector<Record>::iterator it=RList.begin();it!=RList.end();it++)
    {
        if(it->Date == Sdate){
            ui->textEdit->append(it->record);
        }
    }
}

void Addevt::on_turnBtn_clicked()
{
    qDebug(__func__);
    qDebug()<<Sdate;
    bool flag = false;
    if(!ui->textEdit->document()->isEmpty()){
        for(vector<Record>::iterator it=RList.begin();it!=RList.end();it++){
            if(it->Date == Sdate){
                    it->record = ui->textEdit->toPlainText();
                    qDebug()<<it->record;
                    flag = true;
            }
        }
        if(!flag){

            rd.Date = Sdate;
            rd.record = ui->textEdit->toPlainText();
            qDebug()<<rd.Date<<"  "<<rd.record;
            RList.push_back(rd);
        }
        saveFile(QString("record.txt"));
        ui->textEdit->clear();
        emit RecordSaved();
    }
    qDebug()<<RList.size();
    this->close();
}

void Addevt::load(QCalendarWidget *ca)
{
    qDebug(__func__);
    QFile file("record.txt");
    if(file.exists()){
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"open faile";
            return;
        }
        QTextStream in(&file);
        QString temp;
        QTextCharFormat chooseFormat;
        chooseFormat.setForeground(Qt::blue);
        while(!in.atEnd()){
            rd.Date = in.readLine();
            qDebug()<<rd.Date;
            QDate t = QDate::fromString(rd.Date);
            ca->setDateTextFormat(t,chooseFormat);
            while((temp=in.readLine())!=""){
                rd.record += temp;
            }
            qDebug()<<rd.record;
            RList.push_back(rd);
            rd.record = "";
        }
        file.close();
    }
}

void Addevt::saveFile(QString fileName)
{
    qDebug(__func__);
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        qDebug()<<"open failed";
        return;
    }
   QTextStream out(&file);
   for(vector<Record>::iterator it=RList.begin();it!=RList.end();it++){
        out << it->Date <<'\n';
        out << it->record <<'\n';
        out << '\n';
    }
}

void Addevt::on_canclBtn_clicked()
{
    ui->textEdit->clear();
    this->close();
}
