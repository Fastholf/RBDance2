#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    controller = new Controller();

    connect(controller, SIGNAL(robotLoaded(int, QString, int)),
            this, SLOT(updateRobotLabel(int, QString, int)));
    FileLoadError error = controller->loadRobotsFromFile();
    if (error != FileLoadErrorNo) {

    }

    connect(controller, SIGNAL(scenarioListLoaded(QVector<QString>)),
            this, SLOT(updateDanceComboBox(QVector<QString>)));
    error = controller->loadScenarioListFromFile();
    if (error != FileLoadErrorNo) {

    }

    connect(controller, SIGNAL(scenarioLoaded(QVector<QString>,
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
        controller->loadScenarioFromFile(index - 1);
    }
    else {
        clearFileNameComboboxes();
    }
}

void MainWindow::on_connect1_pushButton_clicked()
{
    controller->connectRobot(0);
}

void MainWindow::on_connect2_pushButton_clicked()
{
    controller->connectRobot(1);
}

void MainWindow::on_connect3_pushButton_clicked()
{
    controller->connectRobot(2);
}

void MainWindow::on_basicPosture1_pushButton_clicked()
{
    controller->robotBasicPosture(0);
}

void MainWindow::on_basicPosture2_pushButton_clicked()
{
    controller->robotBasicPosture(1);
}

void MainWindow::on_basicPosture3_pushButton_clicked()
{
    controller->robotBasicPosture(2);
}

void MainWindow::on_DCOn1_pushButton_clicked()
{
    controller->robotTurnDCOn(0);
}

void MainWindow::on_DCOn2_pushButton_clicked()
{
    controller->robotTurnDCOn(1);
}

void MainWindow::on_DCOn3_pushButton_clicked()
{
    controller->robotTurnDCOn(2);
}

void MainWindow::on_DCOff1_pushButton_clicked()
{
    controller->robotTurnDCOff(0);
}

void MainWindow::on_DCOff2_pushButton_clicked()
{
    controller->robotTurnDCOff(1);
}

void MainWindow::on_DCOff3_pushButton_clicked()
{
    controller->robotTurnDCOff(2);
}

void MainWindow::on_disconnect1_pushButton_clicked()
{
    controller->robotDisconnect(0);
}

void MainWindow::on_disconnect2_pushButton_clicked()
{
    controller->robotDisconnect(1);
}

void MainWindow::on_disconnect3_pushButton_clicked()
{
    controller->robotDisconnect(2);
}

void MainWindow::on_start_pushButton_clicked()
{
    if (controller->isDanceReady()) {
        controller->danceStart();
    }
}

void MainWindow::on_pause_pushButton_clicked()
{
    controller->dancePause();
}

void MainWindow::on_stop_pushButton_clicked()
{
    controller->danceStop();
}
