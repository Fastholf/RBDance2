#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Controller *controller;

private slots:
    void updateRobotLabel(int index, QString robotName, int portNum);
    void updateDanceComboBox(QVector<QString> scenarioPaths);
};

#endif // MAINWINDOW_H
