#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class MyLabel : public QLabel {
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);
    virtual int heightForWidth( int width ) const;
    virtual QSize sizeHint() const;
    QPixmap scaledPixmap() const;
public slots:
    void setPixmap ( const QPixmap & );
    void resizeEvent(QResizeEvent *);
private:
    QPixmap pix;
};

#endif // MYLABEL_H
