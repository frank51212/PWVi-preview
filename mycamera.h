#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QObject>
#include <myimage.h>
#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>

typedef Pylon::CBaslerUsbInstantCamera Camera_t;
typedef Pylon::CBaslerUsbGrabResultPtr GrabResultPtr_t;

class MyCamera : public QObject{
    Q_OBJECT
public:
    explicit MyCamera(QObject *parent = 0);
    ~MyCamera();
    bool connect();
    void open();
    void close();
    void stopGrabbing();
    bool isConnected();
    bool isOpen();

public slots:
    void startGrabbing();

signals:
    void grabDone(QPixmap imPixmap);
    void finished();
    void error(QString err);

private:
    bool stopped();
    Camera_t *camera;
    GrabResultPtr_t ptrGrabResult;
    bool stopping, connected, opened;
    QMutex mutex;
};

#endif // MYCAMERA_H
