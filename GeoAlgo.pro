#-------------------------------------------------
#
# Project created by QtCreator 2013-09-12T03:27:41
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeoAlgo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplay.cpp \
    gasket.cpp \
    maillage2D.cpp \
    myiterators.cpp \
    geometrie.cpp

HEADERS  += mainwindow.h \
    gldisplay.h \
    gasket.h \
    geometrie.h \
    maillage2D.h \
    myiterators.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11
