#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T10:57:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

VERSION = 0.1.0
TARGET = RBDance2
TEMPLATE = app


SOURCES +=\
        MainWindow.cpp \
    Main.cpp \
    Robot.cpp \
    Scenario.cpp \
    RBController.cpp \
    AppManager.cpp \
    DanceScript.cpp \
    Choreographer.cpp

HEADERS  += MainWindow.h \
    Robot.h \
    Scenario.h \
    RBController.h \
    AppManager.h \
    DanceScript.h \
    Choreographer.h

FORMS    += MainWindow.ui
