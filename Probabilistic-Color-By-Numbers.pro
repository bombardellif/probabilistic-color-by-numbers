#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T13:29:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Probabilistic-Color-By-Numbers
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    color.cpp \
    segment.cpp \
    colorgroup.cpp \
    segmentedimage.cpp \
    statisticscollector.cpp \
    distribution.cpp

HEADERS  += mainwindow.h \
    color.h \
    segment.h \
    colorgroup.h \
    segmentedimage.h \
    distribution.h \
    statisticscollector.h

FORMS    += mainwindow.ui
