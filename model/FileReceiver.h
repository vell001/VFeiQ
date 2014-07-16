#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QObject>

class FileReceiver : public QObject
{
    Q_OBJECT
public:
    explicit FileReceiver(QObject *parent = 0);

signals:

public slots:

};

#endif // FILERECEIVER_H
