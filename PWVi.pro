#-------------------------------------------------
#
# Project created by QtCreator 2016-06-16T14:34:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PWVi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    patient.cpp \
    select.cpp \
    next.cpp \
    wait.cpp \
    mylineedit.cpp \
    myimage.cpp \
    mycamera.cpp \
    mylabel.cpp

HEADERS  += mainwindow.h \
    patient.h \
    select.h \
    next.h \
    wait.h \
    mylineedit.h \
    myimage.h \
    mycamera.h \
    mylabel.h

FORMS    += mainwindow.ui \
    patient.ui \
    select.ui \
    next.ui \
    wait.ui

unix|win32: LIBS += -L$$(PYLON_DEV_DIR)/lib/x64/ \
    -lGCBase_MD_VC120_v3_0_Basler_pylon_v5_0 \
    -lGenApi_MD_VC120_v3_0_Basler_pylon_v5_0 \
    -lPylonBase_MD_VC120_v5_0 \
    -lPylonC_MD_VC120 \
    -lPylonGUI_MD_VC120_v5_0 \
    -lPylonUtility_MD_VC120_v5_0

INCLUDEPATH += $$(PYLON_DEV_DIR)/include
DEPENDPATH += $$(PYLON_DEV_DIR)/include
