#include "Image.h"

Image::Image()
{
}
Image::Image(const QImage &image) :
    QImage(image)
{
}

QString Image::toBase64Data(char *format , bool isCompress ){
    QByteArray ba;
    QBuffer buf(&ba);
    this->save(&buf, format); //解码保存数据

    QByteArray hh;
    if(isCompress) {
        QByteArray cc = qCompress(ba,1);
        hh = cc.toBase64(); //base64数据
    } else {
        hh = ba.toBase64();
    }

    QString str(hh);
    return str;
}

Image Image::fromBase64Data(const QString &fromData, bool isCompress ){
    QByteArray rc;
    rc=QByteArray::fromBase64(fromData.toAscii());
    Image img;
    if(isCompress) {
        QByteArray rdc=qUncompress(rc);
        img.loadFromData(rdc);
    } else {
        img.loadFromData(rc);
    }
    return img;
}
