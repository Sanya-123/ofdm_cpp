#-------------------------------------------------
#
# Project created by QtCreator 2020-12-04T14:02:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = testOFDM
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        ofdm_window.cpp \
    my_demap_modulation.cpp \
    my_fft.cpp \
    my_map_modulation.cpp \
    my_ofdm_payload_gen.cpp \
    my_ofdm_payload_recyve.cpp \
    my_ofdm_frame_gen.cpp \
    qcustomplot.cpp \
    PB.cpp \
    PA.cpp \
    my_preambles.cpp \
    my_ofdm_frame_recive.cpp

HEADERS  += ofdm_window.h \
    common_ofdm.h \
    my_demap_modulation.h \
    my_fft.h \
    my_map_modulation.h \
    my_ofdm_payload_gen.h \
    my_ofdm_payload_recyve.h \
    my_ofdm_frame_gen.h \
    qcustomplot.h \
    my_preambles.h \
    my_ofdm_frame_recive.h \
    my_math_operations.h

FORMS    += ofdm_window.ui
