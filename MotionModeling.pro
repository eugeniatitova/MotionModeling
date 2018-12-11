#-------------------------------------------------
#
# Project created by QtCreator 2018-05-01T22:26:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MotionModeling
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    spline.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    spline.h

FORMS    += mainwindow.ui

RESOURCES += \
    image.qrc
