#include "mycamera.h"
#include <QCoreApplication>

MyCamera::MyCamera(QObject *parent) : QObject(parent), stopping(false), connected(false), opened(false) {
    try {
        Pylon::PylonInitialize();
        camera = new Camera_t();
    }
    catch (...) {
        qDebug("An exception occurred during camera construction.");
    }
}

MyCamera::~MyCamera() {
    stopGrabbing();
    if (isOpen())
        close();
    delete camera;
    Pylon::PylonTerminate();
}

bool MyCamera::connect() {
    try {
        Pylon::CTlFactory& TlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t devices;
        if (TlFactory.EnumerateDevices(devices) == 0)
            throw RUNTIME_EXCEPTION("No device attached.");
        Pylon::CDeviceInfo info;
        info.SetDeviceClass(Camera_t::DeviceClass());
        camera->Attach(TlFactory.CreateFirstDevice(info));
        connected = true;
    }
    catch (GenICam::GenericException &e) {
        qDebug("An exception occurred:");
        qDebug(e.GetDescription());
    }
    catch (...) {
        qDebug("An exception occurred during camera connection.");
    }

    return connected;
}

void MyCamera::open() {
    try {
        camera->Open();
    }
    catch (...) {
        qDebug("An exception occurred during camera opening.");
    }
}

void MyCamera::close() {
    camera->Close(); // should not throw exceptions
}

void MyCamera::startGrabbing() {
    stopping = false;
    while (!stopped()) {
        if (camera->GrabOne(1000, ptrGrabResult, Pylon::ETimeoutHandling::TimeoutHandling_Return)) { // should not throw exceptions
            MyImage *im = new MyImage(ptrGrabResult);
            emit grabDone(im->toQPixmap());
        }
    }
    emit finished();
}

void MyCamera::stopGrabbing() {
    QMutexLocker locker(&mutex);
    stopping = true;
}

bool MyCamera::isConnected() {
    connected = camera->IsPylonDeviceAttached(); // does not throw exceptions
    return connected;
}

bool MyCamera::isOpen() {
    opened = camera->IsOpen(); // does not throw exceptions
    return opened;
}

bool MyCamera::stopped() {
    QMutexLocker locker(&mutex);
    return stopping;
}
