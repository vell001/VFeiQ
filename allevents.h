#ifndef ALLEVENTS_H
#define ALLEVENTS_H
//#include"qschedule.h"
#include"addevt.h"
#include <QWidget>
#include"Record.h"
#include"vector"

namespace Ui {
class AllEvents;
}

class AllEvents : public QWidget
{
    Q_OBJECT

public:
    explicit AllEvents(QWidget *parent = 0);
    ~AllEvents();
    void shows(vector<Record> RList);

private slots:
    void on_closeBtn_clicked();

private:
    Ui::AllEvents *ui;
};

#endif // ALLEVENTS_H
