#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "AppManager.h"

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
    AppManager *appManager;
    QVector<QPushButton*> connectButtons;
    QVector<QPushButton*> basicPustureButtons;
    QVector<QPushButton*> dcOnButtons;
    QVector<QPushButton*> dcOffButtons;
    QVector<QPushButton*> disconnectButtons;

    void fillUIArrays();
    void clearFileNameComboboxes();

    void onRobotConnected(int index);
    void onDCModeTurnedOn(int index);
    void onDCModeTurnedOff(int index);
    void onRobotDisconnected(int index);
    void onDanceStarted();
    void onDancePaused();
    void onDanceStopped();

    void connectRobot(int index);
    void turnDCOn(int index);
    void turnDCOff(int index);
    void disconnectRobot(int index);

private slots:
    void updateRobotLabel(int index, QString robotName, int portNum);
    void updateDanceComboBox(QVector<QString> scenarioPaths);
    void updateFileNameComboBoxes(QVector<QString> danceFileNames,
                                  QVector<Role> roles);
    void onDanceFinished();

    void showMessage(QString message);

    void on_dance_comboBox_currentIndexChanged(int index);
    void on_connect1_pushButton_clicked();
    void on_connect2_pushButton_clicked();
    void on_connect3_pushButton_clicked();
    void on_basicPosture1_pushButton_clicked();
    void on_basicPosture2_pushButton_clicked();
    void on_basicPosture3_pushButton_clicked();
    void on_DCOn1_pushButton_clicked();
    void on_DCOn2_pushButton_clicked();
    void on_DCOn3_pushButton_clicked();
    void on_DCOff1_pushButton_clicked();
    void on_DCOff2_pushButton_clicked();
    void on_DCOff3_pushButton_clicked();
    void on_disconnect1_pushButton_clicked();
    void on_disconnect2_pushButton_clicked();
    void on_disconnect3_pushButton_clicked();
    void on_fileName1_comboBox_currentIndexChanged(int index);
    void on_fileName2_comboBox_currentIndexChanged(int index);
    void on_fileName3_comboBox_currentIndexChanged(int index);
    void on_start_pushButton_clicked();
    void on_pause_pushButton_clicked();
    void on_stop_pushButton_clicked();
    void on_music_checkBox_stateChanged(int arg1);
};

#endif // MAINWINDOW_H
