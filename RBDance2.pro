#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T10:57:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport multimedia

VERSION = 1.4.3
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
    Choreographer.cpp \
    MusicPlayer.cpp \
    SerialPortWorker.cpp

HEADERS  += MainWindow.h \
    Robot.h \
    Scenario.h \
    RBController.h \
    AppManager.h \
    DanceScript.h \
    Choreographer.h \
    MusicPlayer.h \
    SerialPortWorker.h

FORMS    += MainWindow.ui
