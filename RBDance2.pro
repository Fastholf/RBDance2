#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T10:57:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport multimedia

VERSION = 1.5.0
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
    MusicPlayer.cpp \
    SerialPortWorker.cpp \
    StopWatch.cpp \
    ScriptPlayer.cpp \
    ChoreographerWorker.cpp \
    Choreographer.cpp

HEADERS  += MainWindow.h \
    Robot.h \
    Scenario.h \
    RBController.h \
    AppManager.h \
    DanceScript.h \
    MusicPlayer.h \
    SerialPortWorker.h \
    StopWatch.h \
    ScriptPlayer.h \
    ChoreographerWorker.h \
    Choreographer.h

FORMS    += MainWindow.ui
