#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T10:57:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = RBDance2
TEMPLATE = app


SOURCES +=\
        MainWindow.cpp \
    Controller.cpp \
    Main.cpp \
    Robot.cpp \
    Scenario.cpp \
    RBController.cpp

HEADERS  += MainWindow.h \
    Controller.h \
    Robot.h \
    Scenario.h \
    RBController.h

FORMS    += MainWindow.ui
