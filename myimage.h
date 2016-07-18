#ifndef MYIMAGE_H
#define MYIMAGE_H
#include <QImage>
#include <QPixmap>
#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>

typedef Pylon::CBaslerUsbGrabResultPtr GrabResultPtr_t;

class MyImage {
public:
    MyImage(GrabResultPtr_t pointer);
    ~MyImage();
    QImage toQImage();
    QPixmap toQPixmap();

private:
    void demosaic();
    uint8_t *data;
    uint16_t *rawData;
    int width, height;
};

#endif // MYIMAGE_H
