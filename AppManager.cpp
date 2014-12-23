#include "AppManager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "MainWindow.h"

AppManager::AppManager()
{
//    qDebug() << "Method name";
}

bool AppManager::init()
{
//    qDebug() << "Method name";

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


    QTextStream in(&settingsFile);
    rootPath = in.readLine();
    robotsFilePath = QDir::cleanPath(rootPath +
                                     QDir::separator() +
                                     in.readLine());
    scenarioListFilePath = QDir::cleanPath(rootPath +
                                           QDir::separator() +
                                           in.readLine());

    scenario = NULL;
    choreographerWorker = NULL;
    musicPlaying = true;

    FileLoadError error = loadRobotsFromFile();
    if (error != FileLoadErrorNo) {
        return false;
    }

    error = loadScenarioListFromFile();
    if (error != FileLoadErrorNo) {
        return false;
    }

    serialPortThread->start();

    scenario = new Scenario(robots.count());

    choreographer = new Choreographer();
    connect(choreographer, SIGNAL(danceLoaded(int, int)),
            this, SLOT(onDanceLoaded(int, int)));
    connect(choreographer, SIGNAL(currentFrameChanged(int, int)),
            this, SLOT(onCurrentFrameChanged(int, int)));

    return true;
}

FileLoadError AppManager::loadRobotsFromFile()
{
//    qDebug() << "Method name";

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
        connect(newRobot, SIGNAL(turnDCOnFinished(int, bool)),
                this, SLOT(onTurnDCOnFinished(int, bool)));
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
//    qDebug() << "Method name";

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
//    qDebug() << "Method name";

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

    /** Assume three types of line in file:
     * file <dance file path>
     * music <music file path>
     * role <robot number> <dance number> */
    scenario->reset();
    QVector<int> robotNums;
    for (int i = 0; i < robots.count(); ++i) {
        if (robots[i]->isConnected()) {
            robotNums.push_back(i);
        }
    }
    for (int i = 0; i < robots.count(); ++i) {
        if (!robots[i]->isConnected()) {
            robotNums.push_back(i);
        }
    }
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
            scenario->setRole(robotNums[robotNum], danceNum);
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

    if (result == FileLoadErrorNo) {
        QString errorMessage = NULL;
        result = scenario->loadDanceScripts(&errorMessage);
        if (result == FileLoadErrorNo) {
            emit scenarioLoaded(scenario->getDanceFileNames(), scenario->getRoles());
            choreographer->load(scenario, robots);
        }
        else {
            qWarning() << errorMessage;
            showMessage(errorMessage);
        }
    }

    return result;
}

void AppManager::setRobotRole(int robotNum, int danceNum)
{
//    qDebug() << "Method name";

    scenario->setRole(robotNum, danceNum);
    choreographer->load(scenario, robots);
}

void AppManager::setMusicPlaying(bool t_musicPlaying)
{
//    qDebug() << "Method name";

    if (scenario != NULL) {
        scenario->setMusicPlaying(t_musicPlaying);
    }
    musicPlaying = t_musicPlaying;
}

bool AppManager::isRobotIndexOk(int index, QString methodName)
{
//    qDebug() << "Method name";

    if (index < 0 || index > robots.count()) {
        qWarning() << methodName << ": Robot index out of bounds.";
        return false;
    }
    return true;
}

void AppManager::connectRobot(int index)
{
//    qDebug() << "Method name";

    if (isRobotIndexOk(index, "Controller::connectRobot")) {
        robots[index]->connectToRB();
    }
}

void AppManager::connectAllRobots()
{
    for (int i = 0; i < robots.count(); ++i) {
        robots[i]->connectToRB();
    }
}

void AppManager::robotBasicPosture(int index)
{
//    qDebug() << "Method name";

    if (isRobotIndexOk(index, "Controller::robotBasicPosture")) {
        robots[index]->basicPosture();
    }
}

void AppManager::robotBasicPostureAll()
{
    for (int i = 0; i < robots.count(); ++i) {
        robots[i]->basicPosture();
    }
}

void AppManager::robotTurnDCOn(int index)
{
//    qDebug() << "Method name";

    if (isRobotIndexOk(index, "Controller::robotTurnDCOn")) {
        robots[index]->turnDCOn();
    }
}

void AppManager::robotTurnDCOnAll()
{
    for (int i = 0; i < robots.count(); ++i) {
        robots[i]->turnDCOn();
    }
}

void AppManager::robotTurnDCOff(int index)
{
//    qDebug() << "Method name";

    if (isRobotIndexOk(index, "Controller::robotTurnDCOff")) {
        robots[index]->turnDCOff();
    }
}

void AppManager::robotTurnDCOffAll()
{
    for (int i = 0; i < robots.count(); ++i) {
        robots[i]->turnDCOff();
    }
}

void AppManager::robotDisconnect(int index)
{
//    qDebug() << "Method name";

    if (isRobotIndexOk(index, "Controller::robotDisconnect")) {
        robots[index]->disconnect();
    }
}

void AppManager::robotDisconnectAll()
{
    for (int i = 0; i < robots.count(); ++i) {
        robots[i]->disconnect();
    }
}

bool AppManager::isDanceReady()
{
//    qDebug() << "Method name";

    if (scenario == NULL) {
        qWarning() << "Scenario is not loaded";
        showMessage("Dance was not chosen.");
        return false;
    }
    QString errorMessage = NULL;
    if (!scenario->isReady(&errorMessage)) {
        qWarning() << errorMessage;
        showMessage(errorMessage);
        return false;
    }

//    For debug purpose only
//    return true;

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
//    qDebug() << "Method name";

    choreographerWorker = new ChoreographerWorker(choreographer);

    connect(choreographerWorker, SIGNAL(danceFinished()),
            this, SLOT(onDanceFinished()));

    choreographerThread = new QThread();
    choreographerWorker->moveToThread(choreographerThread);
    connect(choreographerThread, SIGNAL(started()),
            choreographerWorker, SLOT(startDance()));
    connect(choreographerWorker, SIGNAL(danceFinished()),
            choreographerThread, SLOT(quit()));
    connect(choreographerWorker, SIGNAL(danceFinished()),
            choreographerWorker, SLOT(deleteLater()));
    connect(choreographerThread, SIGNAL(finished()),
            choreographerThread, SLOT(deleteLater()));
    choreographerThread->start();
}

void AppManager::dancePause()
{
//    qDebug() << "Method name";

    if (choreographerWorker == NULL) {
        return;
    }
    choreographerWorker->pauseDance();
}

void AppManager::danceStop()
{
//    qDebug() << "Method name";

    if (choreographerWorker == NULL) {
        return;
    }
    choreographerWorker->stopDance();
}

void AppManager::onDanceFinished()
{
//    qDebug() << "Method name";

    if (choreographerWorker != NULL) {
        choreographerWorker = NULL;
    }
    danceFinished();
}

void AppManager::onConnectTryFinished(int index, bool result)
{
//    qDebug() << "Method name";

    emit connectTryFinished(index, result);
}

void AppManager::onTurnDCOnFinished(int index, bool result)
{
//    qDebug() << "Method name";

    emit turnDCOnFinished(index, result);
}

void AppManager::onTurnDCOffFinished(int index)
{
//    qDebug() << "Method name";

    emit turnDCOffFinished(index);
}

void AppManager::onDisconnected(int index)
{
//    qDebug() << "Method name";

    emit disconnected(index);
}

void AppManager::onDanceLoaded(int maxIndex, int duration)
{
//    qDebug() << "Method name";

    emit danceLoaded(maxIndex, duration);
}

void AppManager::onCurrentFrameChanged(int index, int elapsedTime)
{
//    qDebug() << "Method name";

    emit currentFrameChanged(index, elapsedTime);
}
