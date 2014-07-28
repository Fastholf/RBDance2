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
    void clearFileNameComboboxes();

private slots:
    void updateRobotLabel(int index, QString robotName, int portNum);
    void updateDanceComboBox(QVector<QString> scenarioPaths);
    void updateFileNameComboBoxes(QVector<QString> danceFileNames,
                                  QVector<Role> roles);
    void on_dance_comboBox_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
