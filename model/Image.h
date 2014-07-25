#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <QString>
#include <QBuffer>

class Image : public QImage
{
public:
    explicit Image();
    explicit Image(const QImage &image);

    QString toBase64Data(char *format = "PNG", bool isCompress = false);

    static Image fromBase64Data(const QString &fromData, bool isCompress = false);
signals:

public slots:
private:

};

#endif // IMAGE_H
