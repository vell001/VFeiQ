#ifndef DELETEVENT_H
#define DELETEVENT_H
#include <QDialog>
#include "addevt.h"

namespace Ui {
class DeletEvent;
}

class DeletEvent : public QDialog
{
    Q_OBJECT

public:
    explicit DeletEvent(vector<Record>& RList,QWidget *parent = 0);
    ~DeletEvent();
    void dlt();
    vector<Record> *RList;

private slots:
    void on_tureBtn_clicked();

    void on_cancleBtn_clicked();

private:
    Ui::DeletEvent *ui;
};

#endif // DELETEVENT_H
