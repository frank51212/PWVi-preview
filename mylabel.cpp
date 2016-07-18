#include "mylabel.h"
//#include <QDebug>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {
    this->setMinimumSize(1,1);
    setScaledContents(false);
}

void MyLabel::setPixmap ( const QPixmap & p) {
    pix = p;
    QLabel::setPixmap(scaledPixmap());
}

int MyLabel::heightForWidth( int width ) const {
    return pix.isNull() ? this->height() : ((qreal)pix.height()*width)/pix.width();
}

QSize MyLabel::sizeHint() const {
    int w = this->width();
    return QSize( w, heightForWidth(w) );
}

QPixmap MyLabel::scaledPixmap() const {
    return pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void MyLabel::resizeEvent(QResizeEvent *) {
    if(!pix.isNull())
        QLabel::setPixmap(scaledPixmap());
}
