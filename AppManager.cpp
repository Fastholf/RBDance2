#include "AppManager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "MainWindow.h"

AppManager::AppManager()
{

}

bool AppManager::init()
{
    settingsFilePath = "settings.txt";
    QFile settingsFile(settingsFilePath);
    if (!settingsFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Settings file was not found.";
        showMessage("Settings file was not found.");
        return false;
    }

    serialPortThread = new QThread;
    serialPortWorker = new SerialPortWorker;
    serialPortWorker->moveToThread(serialPortThread);
    connect(serialPortThread, SIGNAL(started()),
            serialPortWorker, SLOT(runQueue()));
    connect(serialPortWorker, SIGNAL(workFinished()),
            serialPortThread, SLOT(quit()));
    connect(serialPortWorker, SIGNAL(workFinished()),
            serialPortWorker, SLOT(deleteLater()));
    connect(serialPortThread, SIGNAL(finished()),
            serialPortThread, SLOT(deleteLater()));
    serialPortThread->start();

    QTextStream in(&settingsFile);
    rootPath = in.readLine();
    robotsFilePath = QDir::cleanPath(rootPath +
                                     QDir::separator() +
                                     in.readLine());
    scenarioListFilePath = QDir::cleanPath(rootPath +
                                           QDir::separator() +
                                           in.readLine());

    scenario = NULL;
    choreographer = NULL;
    musicPlaying = true;

    FileLoadError error = loadRobotsFromFile();
    if (error != FileLoadErrorNo) {
        return false;
    }

    error = loadScenarioListFromFile();
    if (error != FileLoadErrorNo) {
        return false;
    }

    return true;
}

FileLoadError AppManager::loadRobotsFromFile()
{
    QFile robotsFile(robotsFilePath);
    if (!robotsFile.open(QIODevice::ReadOnly)) {
        qCritical() << "File with robots description was not found.";
        showMessage("File with robots description was not found.");
        return FileLoadErrorNotFound;
    }
    FileLoadError result = FileLoadErrorNo;
    QTextStream in(&robotsFile);

    /** Assume the following format of the line in file:
    * <robot name> <serial number> <port number> <bluetooth name> **/
    int i = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList items = line.split(" ");
        if (items.size() < 4) {
            result = FileLoadErrorWrongFormat;
            qCritical() << "File with robots description has wrong format. \
                           Not enough items in the line.";
            showMessage("File with robots description has wrong format.");
            break;
        }
        QString robotName = items[0];
        QString sPortNum = items[2];
        bool ok;
        int portNum = sPortNum.toInt(&ok);
        if (!ok) {
            result = FileLoadErrorWrongFormat;
            qCritical() << "File with robots description has wrong format. \
                           Port num is not integer.";
            showMessage("File with robots description has wrong format.");
            break;
        }
        Robot *newRobot = new Robot(i, portNum, robotName, serialPortWorker);
        connect(newRobot, SIGNAL(connectTryFinished(int, bool)),
                this, SLOT(onConnectTryFinished(int, bool)));
        connect(newRobot, SIGNAL(turnDCOnFinished(int,bool)),
                this, SLOT(onTurnDCOnFinished(int,bool)));
        connect(newRobot, SIGNAL(turnDCOffFinished(int)),
                this, SLOT(onTurnDCOffFinished(int)));
        connect(newRobot, SIGNAL(disconnected(int)),
                this, SLOT(onDisconnected(int)));
        robots.push_back(newRobot);
        serialPortWorker->addSerialPort(portNum);
        emit robotLoaded(i, robotName, portNum);
        ++i;
    }
    robotsFile.close();

    return result;
}

FileLoadError AppManager::loadScenarioListFromFile()
{
    QFile scenarioListFile(scenarioListFilePath);
    if (!scenarioListFile.open(QIODevice::ReadOnly)) {
        qCritical() << "File with the list of scenarios was not found.";
        showMessage("File with the list of scenarios was not found.");
        return FileLoadErrorNotFound;
    }
    FileLoadError result = FileLoadErrorNo;
    QTextStream in(&scenarioListFile);

    while (!in.atEnd()) {
        QString line = in.readLine();
        scenarioPaths.push_back(line);
    }
    scenarioListFile.close();

    scenarioListLoaded(scenarioPaths);

    return result;
}

FileLoadError AppManager::loadScenarioFromFile(int scenarioIndex)
{

    QFile scenarioFile(QDir::cleanPath(rootPath +
                                       QDir::separator() +
                                       scenarioPaths[scenarioIndex]));
    if (!scenarioFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Scenario file was not found.";
        showMessage("Scenario file was not found.");
        return FileLoadErrorNotFound;
    }
    FileLoadError result = FileLoadErrorNo;
    QTextStream in(&scenarioFile);
    QFileInfo fileInfo(scenarioFile.fileName());
    QString scenarioPath(fileInfo.path());
    qDebug() << scenarioPath;

    /** Assume three types of line in file:
     * file <dance file path>
     * music <music file path>
     * role <robot number> <dance number> */
    scenario = new Scenario(robots.count());
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line[0] == '#') { // Comment
            continue;
        }
        QStringList items = line.split(" ");
        if (items.size() < 2) {
            result = FileLoadErrorWrongFormat;
            qCritical() << "Scenario file has wrong format. \
                           Not enough items in the line.";
            showMessage("Scenario file has wrong format.");
            break;
        }
        if (items.size() > 3) {
            result = FileLoadErrorWrongFormat;
            qCritical() << "Scenario file has wrong format. \
                           Too many items in the line.";
            showMessage("Scenario file has wrong format.");
            break;
        }
        QString command = items[0];
        if (command == "file") {
            scenario->addDanceFile(QDir::cleanPath(scenarioPath +
                                                   QDir::separator() +
                                                   items[1]));
        }
        else if (command == "role") {
            /** Assume following format of command
             * role <robot number> <file number> */
            bool ok;
            int robotNum = items[1].toInt(&ok);
            if (!ok) {
                result = FileLoadErrorWrongFormat;
                qCritical() << "Scenario file has wrong format. \
                               Robot num is not integer.";
                break;
            }
            int danceNum = items[2].toInt(&ok);
            if (!ok) {
                result = FileLoadErrorWrongFormat;
                qCritical() << "Scenario file has wrong format. \
                               Dance num is not integer.";
            showMessage("Scenario file has wrong format.");
                break;
            }
            scenario->setRole(robotNum, danceNum);
        }
        else if (command == "music") {
            scenario->setMusic(QDir::cleanPath(scenarioPath +
                                               QDir::separator() +
                                               items[1]));
        }
        else {
            result = FileLoadErrorWrongFormat;
            qCritical() << "Scenario file has wrong format. \
                           Unknown command.";
            showMessage("Scenario file has wrong format.");
            break;
        }
    }
    scenario->setMusicPlaying(musicPlaying);
    scenarioFile.close();

    scenarioLoaded(scenario->getDanceFileNames(), scenario->getRoles());

    return result;
}

void AppManager::setRobotRole(int robotNum, int danceNum)
{
    scenario->setRole(robotNum, danceNum);
}

void AppManager::setMusicPlaying(bool t_musicPlaying)
{
    if (scenario != NULL) {
        scenario->setMusicPlaying(t_musicPlaying);
    }
    musicPlaying = t_musicPlaying;
}

bool AppManager::isRobotIndexOk(int index, QString methodName)
{
    if (index < 0 || index > robots.count()) {
        qWarning() << methodName << ": Robot index out of bounds.";
        return false;
    }
    return true;
}

void AppManager::connectRobot(int index)
{
    if (isRobotIndexOk(index, "Controller::connectRobot")) {
        robots[index]->connectToRB();
    }
}

void AppManager::robotBasicPosture(int index)
{
    if (isRobotIndexOk(index, "Controller::robotBasicPosture")) {
        robots[index]->basicPosture();
    }
}

void AppManager::robotTurnDCOn(int index)
{
    if (isRobotIndexOk(index, "Controller::robotTurnDCOn")) {
        robots[index]->turnDCOn();
    }
}

void AppManager::robotTurnDCOff(int index)
{
    if (isRobotIndexOk(index, "Controller::robotTurnDCOff")) {
        robots[index]->turnDCOff();
    }
}

void AppManager::robotDisconnect(int index)
{
    if (isRobotIndexOk(index, "Controller::robotDisconnect")) {
        robots[index]->disconnect();
    }
}

bool AppManager::isDanceReady()
{
    if (scenario == NULL) {
        qWarning() << "Scenario is not loaded";
        showMessage("Dance was not chosen.");
        return false;
    }
    QString errorMessage = NULL;
    if (!scenario->loadDanceScripts(&errorMessage)) {
        qWarning() << errorMessage;
        showMessage(errorMessage);
        return false;
    }
    if (!scenario->isMusicReady()) {
        qWarning() << "Music file was not found.";
        showMessage("Music file was not found.");
        return false;
    }
    QVector<int> involvedRobotNums = scenario->involvedRobotNums();
    for (int i = 0; i < involvedRobotNums.count(); ++i) {
        int rNum = involvedRobotNums[i];
        if (!robots[rNum]->isConnected()) {
            qWarning() << "Robot " + robots[rNum]->getName() +
                          " is not connected!";
            showMessage("Robot " + robots[rNum]->getName() +
                        " is not connected!");
            return false;
        }
        if (!robots[rNum]->isDCModeOn()) {
            qWarning() << "Robot " + robots[rNum]->getName() +
                          " is not in DC mode!";
            showMessage("Robot " + robots[rNum]->getName() +
                        " is not DC mode!");
            return false;
        }
    }
    return true;
}

void AppManager::danceStart()
{
    qDebug() << "AppManager::danceStart: " << QThread::currentThread();
    choreographer = new Choreographer();

    connect(choreographer, SIGNAL(danceFinished()),
            this, SLOT(onDanceFinished()));
    choreographer->moveToThread(serialPortThread);
    choreographer->setRobots(robots);
    choreographer->setScenario(scenario);
    connect(serialPortThread, SIGNAL(started()),
            choreographer, SLOT(startDance()));
    serialPortThread->start();
}

void AppManager::dancePause()
{
    if (choreographer == NULL) {
        return;
    }
    choreographer->pauseDance();
}

void AppManager::danceStop()
{
    if (choreographer == NULL) {
        return;
    }
    choreographer->stopDance();
}

void AppManager::onDanceFinished()
{
    if (choreographer != NULL) {
        delete choreographer;
        choreographer = NULL;
    }
    danceFinished();
}

void AppManager::onConnectTryFinished(int index, bool result)
{
    emit connectTryFinished(index, result);
}

void AppManager::onTurnDCOnFinished(int index, bool result)
{
    emit turnDCOnFinished(index, result);
}

void AppManager::onTurnDCOffFinished(int index)
{
    emit turnDCOffFinished(index);
}

void AppManager::onDisconnected(int index)
{
    emit disconnected(index);
}
