#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    appManager = new AppManager();

    connect(appManager, SIGNAL(robotLoaded(int, QString, int)),
            this, SLOT(updateRobotLabel(int, QString, int)));
    FileLoadError error = appManager->loadRobotsFromFile();
    if (error != FileLoadErrorNo) {

    }

    connect(appManager, SIGNAL(scenarioListLoaded(QVector<QString>)),
            this, SLOT(updateDanceComboBox(QVector<QString>)));
    error = appManager->loadScenarioListFromFile();
    if (error != FileLoadErrorNo) {

    }

    connect(appManager, SIGNAL(scenarioLoaded(QVector<QString>,
                                              QVector<Role>)),
            this, SLOT(updateFileNameComboBoxes(QVector<QString>,
                                                QVector<Role>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearFileNameComboboxes()
{
    ui->fileName1_comboBox->clear();
    ui->fileName2_comboBox->clear();
    ui->fileName3_comboBox->clear();
}

void MainWindow::updateRobotLabel(int index, QString robotName, int portNum)
{
    QLabel *robotLabel;
    switch (index) {
        case 0: robotLabel = ui->robotName1_label; break;
        case 1: robotLabel = ui->robotName2_label; break;
        case 2: robotLabel = ui->robotName3_label; break;
    }
    robotLabel->setText(robotName + " " + QString::number(portNum));
}

void MainWindow::updateDanceComboBox(QVector<QString> scenarioPaths)
{
    ui->dance_comboBox->clear();
    ui->dance_comboBox->addItem("None");
    for (int i = 0; i < scenarioPaths.count(); ++i) {
        ui->dance_comboBox->addItem(scenarioPaths[i]);
    }
}

void MainWindow::updateFileNameComboBoxes(QVector<QString> danceFileNames,
                                          QVector<Role> roles)
{
    QVector<QComboBox*> comboBoxes;
    comboBoxes.push_back(ui->fileName1_comboBox);
    comboBoxes.push_back(ui->fileName2_comboBox);
    comboBoxes.push_back(ui->fileName3_comboBox);
    for (int i = 0; i < comboBoxes.count(); ++i) {
        comboBoxes[i]->clear();
        comboBoxes[i]->addItem("None");
        for (int j = 0; j < danceFileNames.count(); ++j) {
            comboBoxes[i]->addItem(danceFileNames[j]);
        }
        for (int j = 0; j < roles.count(); ++j) {
            if (roles[j].robotNum == i) {
                comboBoxes[i]->setCurrentIndex(roles[j].danceNum + 1);
            }
        }
    }
}

void MainWindow::on_dance_comboBox_currentIndexChanged(int index)
{
    if (index > 0) {
        appManager->loadScenarioFromFile(index - 1);
    }
    else {
        clearFileNameComboboxes();
    }
}

void MainWindow::on_connect1_pushButton_clicked()
{
    appManager->connectRobot(0);
}

void MainWindow::on_connect2_pushButton_clicked()
{
    appManager->connectRobot(1);
}

void MainWindow::on_connect3_pushButton_clicked()
{
    appManager->connectRobot(2);
}

void MainWindow::on_basicPosture1_pushButton_clicked()
{
    appManager->robotBasicPosture(0);
}

void MainWindow::on_basicPosture2_pushButton_clicked()
{
    appManager->robotBasicPosture(1);
}

void MainWindow::on_basicPosture3_pushButton_clicked()
{
    appManager->robotBasicPosture(2);
}

void MainWindow::on_DCOn1_pushButton_clicked()
{
    appManager->robotTurnDCOn(0);
}

void MainWindow::on_DCOn2_pushButton_clicked()
{
    appManager->robotTurnDCOn(1);
}

void MainWindow::on_DCOn3_pushButton_clicked()
{
    appManager->robotTurnDCOn(2);
}

void MainWindow::on_DCOff1_pushButton_clicked()
{
    appManager->robotTurnDCOff(0);
}

void MainWindow::on_DCOff2_pushButton_clicked()
{
    appManager->robotTurnDCOff(1);
}

void MainWindow::on_DCOff3_pushButton_clicked()
{
    appManager->robotTurnDCOff(2);
}

void MainWindow::on_disconnect1_pushButton_clicked()
{
    appManager->robotDisconnect(0);
}

void MainWindow::on_disconnect2_pushButton_clicked()
{
    appManager->robotDisconnect(1);
}

void MainWindow::on_disconnect3_pushButton_clicked()
{
    appManager->robotDisconnect(2);
}

void MainWindow::on_fileName1_comboBox_currentIndexChanged(int index)
{
    appManager->setRobotRole(0, index - 1);
}

void MainWindow::on_fileName2_comboBox_currentIndexChanged(int index)
{
    appManager->setRobotRole(1, index - 1);
}

void MainWindow::on_fileName3_comboBox_currentIndexChanged(int index)
{
    appManager->setRobotRole(2, index - 1);
}

void MainWindow::on_start_pushButton_clicked()
{
    if (appManager->isDanceReady()) {
        appManager->danceStart();
    }
}

void MainWindow::on_pause_pushButton_clicked()
{
    appManager->dancePause();
}

void MainWindow::on_stop_pushButton_clicked()
{
    appManager->danceStop();
}
