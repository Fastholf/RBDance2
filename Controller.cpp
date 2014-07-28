#include "Controller.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "MainWindow.h"

Controller::Controller()
{
    robotsFileName = "dancers.txt";
    scenarioListFileName = "scenarios.txt";
}

FileLoadError Controller::loadRobotsFromFile()
{
    QFile robotsFile(robotsFileName);
    if (!robotsFile.open(QIODevice::ReadOnly)) {
        qCritical() << "File with robots description was not found.";
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
            break;
        }
        Robot newRobot = Robot(portNum, robotName);
        robots.push_back(newRobot);
        robotLoaded(i, robotName, portNum);
        ++i;
    }
    robotsFile.close();

    return result;
}

FileLoadError Controller::loadScenarioListFromFile()
{
    QFile scenarioListFile(scenarioListFileName);
    if (!scenarioListFile.open(QIODevice::ReadOnly)) {
        qCritical() << "File with the list of scenarios was not found.";
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

FileLoadError Controller::loadScenarioFromFile(int scenarioIndex)
{
    QFile scenarioFile(scenarioPaths[scenarioIndex]);
    if (!scenarioFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Scenario file was not found.";
        return FileLoadErrorNotFound;
    }
    FileLoadError result = FileLoadErrorNo;
    QTextStream in(&scenarioFile);

    /** Assume three type of line in file:
     * file <dance file path>
     * music <music file path>
     * role <robot number> <dance number> */
    scenario = new Scenario();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line[0] == '#') {
            continue;
        }
        QStringList items = line.split(" ");
        if (items.size() < 2) {
            result = FileLoadErrorWrongFormat;
            qCritical() << "Scenario file has wrong format. \
                           Not enough items in the line.";
            break;
        }
        if (items.size() > 3) {
            result = FileLoadErrorWrongFormat;
            qCritical() << "Scenario file has wrong format. \
                           Too many items in the line.";
            break;
        }
        QString command = items[0];
        if (command == "file") {
            scenario->addDanceFile(items[1]);
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
                break;
            }
            scenario->addRole(robotNum, danceNum);
        }
        else if (command == "music") {
            scenario->setMusic(items[1]);
        }
        else {
            result = FileLoadErrorWrongFormat;
            qCritical() << "Scenario file has wrong format. \
                           Unknown command.";
            break;
        }
    }
    scenarioFile.close();

    scenarioLoaded(scenario->getDanceFileNames(), scenario->getRoles());

    return result;
}
