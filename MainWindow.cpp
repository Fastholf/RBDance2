#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fillUIArrays();

    appManager = new AppManager();


    connect(appManager, SIGNAL(robotLoaded(int, QString, int)),
            this, SLOT(updateRobotLabel(int, QString, int)));

    connect(appManager, SIGNAL(scenarioListLoaded(QVector<QString>)),
            this, SLOT(updateDanceComboBox(QVector<QString>)));

    connect(appManager, SIGNAL(scenarioLoaded(QVector<QString>,
                                              QVector<Role>)),
            this, SLOT(updateFileNameComboBoxes(QVector<QString>,
                                                QVector<Role>)));

    connect(appManager, SIGNAL(danceLoaded(int, int)),
            this, SLOT(onDanceLoaded(int, int)));
    connect(appManager, SIGNAL(currentFrameChanged(int, int)),
            this, SLOT(onCurrentFrameChanged(int, int)));
    connect(appManager, SIGNAL(danceFinished()),
            this, SLOT(onDanceFinished()));

    connect(appManager, SIGNAL(showMessage(QString)),
            this, SLOT(showMessage(QString)));

    connect(appManager, SIGNAL(connectTryFinished(int, bool)),
            this, SLOT(onConnectTryFinished(int, bool)));
    connect(appManager, SIGNAL(turnDCOnFinished(int, bool)),
            this, SLOT(onTurnDCOnFinished(int, bool)));
    connect(appManager, SIGNAL(turnDCOffFinished(int)),
            this, SLOT(onTurnDCOffFinished(int)));
    connect(appManager, SIGNAL(disconnected(int)),
            this, SLOT(onDisconnected(int)));

    if (!appManager->init()) {
        ui->dance_prepare_widget->setEnabled(false);
        ui->start_pushButton->setEnabled(false);
        ui->pause_pushButton->setEnabled(false);
        ui->stop_pushButton->setEnabled(false);
        return;
    }

    scenarioLoading = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillUIArrays()
{
    connectButtons.push_back(ui->connect1_pushButton);
    connectButtons.push_back(ui->connect2_pushButton);
    connectButtons.push_back(ui->connect3_pushButton);

    basicPustureButtons.push_back(ui->basicPosture1_pushButton);
    basicPustureButtons.push_back(ui->basicPosture2_pushButton);
    basicPustureButtons.push_back(ui->basicPosture3_pushButton);

    dcOnButtons.push_back(ui->DCOn1_pushButton);
    dcOnButtons.push_back(ui->DCOn2_pushButton);
    dcOnButtons.push_back(ui->DCOn3_pushButton);

    dcOffButtons.push_back(ui->DCOff1_pushButton);
    dcOffButtons.push_back(ui->DCOff2_pushButton);
    dcOffButtons.push_back(ui->DCOff3_pushButton);

    disconnectButtons.push_back(ui->disconnect1_pushButton);
    disconnectButtons.push_back(ui->disconnect2_pushButton);
    disconnectButtons.push_back(ui->disconnect3_pushButton);

    fileNameComboBoxes.push_back(ui->fileName1_comboBox);
    fileNameComboBoxes.push_back(ui->fileName2_comboBox);
    fileNameComboBoxes.push_back(ui->fileName3_comboBox);
}

void MainWindow::clearFileNameComboboxes()
{
    scenarioLoading = true;
    for (int i = 0; i < fileNameComboBoxes.count(); ++i) {
        fileNameComboBoxes[i]->clear();
    }
    scenarioLoading = false;
}

void MainWindow::onRobotConnected(int index)
{
    connectButtons[index]->setEnabled(false);
    basicPustureButtons[index]->setEnabled(true);
    dcOnButtons[index]->setEnabled(true);
    disconnectButtons[index]->setEnabled(true);
}

void MainWindow::onDCModeTurnedOn(int index)
{
    basicPustureButtons[index]->setEnabled(false);
    dcOnButtons[index]->setEnabled(false);
    dcOffButtons[index]->setEnabled(true);
}

void MainWindow::onDCModeTurnedOff(int index)
{
    basicPustureButtons[index]->setEnabled(true);
    dcOnButtons[index]->setEnabled(true);
    dcOffButtons[index]->setEnabled(false);
}

void MainWindow::onRobotDisconnected(int index)
{
    connectButtons[index]->setEnabled(true);
    basicPustureButtons[index]->setEnabled(false);
    dcOnButtons[index]->setEnabled(false);
    dcOffButtons[index]->setEnabled(false);
    disconnectButtons[index]->setEnabled(false);
}

void MainWindow::onDanceStarted()
{
    ui->dance_prepare_widget->setEnabled(false);
    ui->start_pushButton->setEnabled(false);
    ui->pause_pushButton->setEnabled(true);
    ui->stop_pushButton->setEnabled(true);
}

void MainWindow::onDancePaused()
{
    if (ui->pause_pushButton->text() == "Pause") {
        ui->pause_pushButton->setText("Unpause");
    }
    else {
        ui->pause_pushButton->setText("Pause");
    }
}

void MainWindow::onDanceStopped()
{
    ui->dance_prepare_widget->setEnabled(true);
    ui->start_pushButton->setEnabled(true);
    ui->pause_pushButton->setEnabled(false);
    ui->stop_pushButton->setEnabled(false);
    ui->pause_pushButton->setText("Pause");
    ui->dance_horizontalSlider->setValue(0);
    ui->duration_label->setText("00:00/" + durationTime);
}

void MainWindow::connectRobot(int index)
{
    connectButtons[index]->setEnabled(false);
    appManager->connectRobot(index);
}

void MainWindow::turnDCOn(int index)
{
    dcOnButtons[index]->setEnabled(false);
    appManager->robotTurnDCOn(index);
}

void MainWindow::turnDCOff(int index)
{
    appManager->robotTurnDCOff(index);
}

void MainWindow::disconnectRobot(int index)
{
    appManager->robotDisconnect(index);
}

void MainWindow::setRobotRole(int index, int danceNum)
{
    if (!scenarioLoading) {
        appManager->setRobotRole(index, danceNum);
    }
}

bool MainWindow::isRobotConnected(int index)
{
    return !connectButtons[index]->isEnabled();
}

void MainWindow::updateRobotLabel(int index, QString robotName, int portNum)
{
    QLabel *robotLabel;
    switch (index) {
        case 0: robotLabel = ui->robotName1_label; break;
        case 1: robotLabel = ui->robotName2_label; break;
        case 2: robotLabel = ui->robotName3_label; break;
    default: return;
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
    scenarioLoading = true;
    for (int i = 0; i < fileNameComboBoxes.count(); ++i) {
        fileNameComboBoxes[i]->clear();
        fileNameComboBoxes[i]->addItem("None");
        for (int j = 0; j < danceFileNames.count(); ++j) {
            fileNameComboBoxes[i]->addItem(danceFileNames[j]);
        }
        for (int j = 0; j < roles.count(); ++j) {
            if (roles[j].robotNum == i) {
                fileNameComboBoxes[i]->setCurrentIndex(roles[j].danceNum + 1);
            }
        }
    }
    scenarioLoading = false;
}

void MainWindow::onConnectTryFinished(int index, bool result)
{
    if (result) {
        onRobotConnected(index);
    }
    else {
        onRobotDisconnected(index);
    }
}

void MainWindow::onTurnDCOnFinished(int index, bool result)
{
    if (result) {
        onDCModeTurnedOn(index);
    }
    else if (isRobotConnected(index)){
        onDCModeTurnedOff(index);
    }
}

void MainWindow::onTurnDCOffFinished(int index)
{
    if (isRobotConnected(index)) {
        onDCModeTurnedOff(index);
    }
}

void MainWindow::onDisconnected(int index)
{
    onRobotDisconnected(index);
}

QString timeStringFromMilliseconds(int milliseconds)
{
    int secs = milliseconds / 1000;
    int mins = secs / 60;
    secs = secs % 60;
    QString sMins, sSecs;
    return sMins.sprintf("%02d", mins) + ":" + sSecs.sprintf("%02d", secs);
}

void MainWindow::onDanceLoaded(int maxIndex, int duration)
{
    ui->dance_horizontalSlider->setMaximum(maxIndex);
    durationTime = timeStringFromMilliseconds(duration);
    ui->duration_label->setText("00:00/" + durationTime);
}

void MainWindow::onCurrentFrameChanged(int index, int elapsedTime)
{
    ui->dance_horizontalSlider->setValue(index);
    QString currentTime = timeStringFromMilliseconds(elapsedTime);
    ui->duration_label->setText(currentTime + "/" + durationTime);
}

void MainWindow::onDanceFinished()
{
    onDanceStopped();
}

void MainWindow::showMessage(QString message)
{
    QMessageBox::warning(this, "Error", message);
}

void MainWindow::on_connect1_pushButton_clicked()
{
    connectRobot(0);
}

void MainWindow::on_connect2_pushButton_clicked()
{
    connectRobot(1);
}

void MainWindow::on_connect3_pushButton_clicked()
{
    connectRobot(2);
}

void MainWindow::on_connectAll_pushButton_clicked()
{
    appManager->connectAllRobots();
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

void MainWindow::on_basicPostureAll_pushButton_clicked()
{
    appManager->robotBasicPostureAll();
}

void MainWindow::on_DCOn1_pushButton_clicked()
{
    turnDCOn(0);
}

void MainWindow::on_DCOn2_pushButton_clicked()
{
    turnDCOn(1);
}

void MainWindow::on_DCOn3_pushButton_clicked()
{
    turnDCOn(2);
}

void MainWindow::on_DCOnAll_pushButton_clicked()
{
    appManager->robotTurnDCOnAll();
}

void MainWindow::on_DCOff1_pushButton_clicked()
{
    turnDCOff(0);
}

void MainWindow::on_DCOff2_pushButton_clicked()
{
    turnDCOff(1);
}

void MainWindow::on_DCOff3_pushButton_clicked()
{
    turnDCOff(2);
}

void MainWindow::on_DCOffAll_pushButton_clicked()
{
    appManager->robotTurnDCOffAll();
}

void MainWindow::on_disconnect1_pushButton_clicked()
{
    disconnectRobot(0);
}

void MainWindow::on_disconnect2_pushButton_clicked()
{
    disconnectRobot(1);
}

void MainWindow::on_disconnect3_pushButton_clicked()
{
    disconnectRobot(2);
}

void MainWindow::on_disconnectAll_pushButton_clicked()
{
    appManager->robotDisconnectAll();
}

void MainWindow::on_fileName1_comboBox_currentIndexChanged(int index)
{
    setRobotRole(0, index - 1);
}

void MainWindow::on_fileName2_comboBox_currentIndexChanged(int index)
{
    setRobotRole(1, index - 1);
}

void MainWindow::on_fileName3_comboBox_currentIndexChanged(int index)
{
    setRobotRole(2, index - 1);
}

void MainWindow::on_start_pushButton_clicked()
{
    if (appManager->isDanceReady()) {
        appManager->danceStart();
        onDanceStarted();
    }
}

void MainWindow::on_pause_pushButton_clicked()
{
    appManager->dancePause();
    onDancePaused();
}

void MainWindow::on_stop_pushButton_clicked()
{
    appManager->danceStop();
    onDanceStopped();
}

void MainWindow::on_music_checkBox_stateChanged(int arg1)
{
    appManager->setMusicPlaying(arg1);
}

void MainWindow::on_dance_comboBox_activated(int index)
{
    if (index > 0) {
        FileLoadError error = appManager->loadScenarioFromFile(index - 1);
        if (error != FileLoadErrorNo) {
            clearFileNameComboboxes();
        }
    }
    else {
        clearFileNameComboboxes();
    }
}
