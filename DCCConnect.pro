QT += core
QT -= gui

CONFIG += c++11

TARGET = DCCConnect
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    fpga.cpp \
    hps.cpp \
    writer.cpp \
    circularq.cpp \
    hamming.cpp

HEADERS += \
    fpga.h \
    hps.h \
    writer.h \
    circularq.h \
    hamming.h


INCLUDEPATH += /home/terasic/intelFPGA/16.1/embedded/ip/altera/hps/altera_hps/hwlib/include
INCLUDEPATH += /home/terasic/intelFPGA/16.1/embedded/ip/altera/hps/altera_hps/hwlib/include/soc_cv_av
DEPENDPATH += /home/terasic/intelFPGA/16.1/embedded/ip/altera/hps/altera_hps/hwlib/include

INCLUDEPATH += /home/nathan/intelFPGA/16.1/embedded/ip/altera/hps/altera_hps/hwlib/include
INCLUDEPATH += /home/nathan/intelFPGA/16.1/embedded/ip/altera/hps/altera_hps/hwlib/include/soc_cv_av
DEPENDPATH += /home/nathan/intelFPGA/16.1/embedded/ip/altera/hps/altera_hps/hwlib/include
