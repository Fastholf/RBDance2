#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    controller = new Controller();
    connect(controller, SIGNAL(robotLoaded(int, QString, int)), this, SLOT(updateRobotLabel(int, QString, int)));
    FileLoadError error = controller->LoadRobotsFromFile();
    if (error != FileLoadErrorNo) {

    }
}

MainWindow::~MainWindow()
{
    delete ui;
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
