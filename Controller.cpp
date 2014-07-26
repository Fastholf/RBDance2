#include "Controller.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "MainWindow.h"

Controller::Controller()
{
    robotsFileName = "dancers.txt";
}

FileLoadError Controller::LoadRobotsFromFile()
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
            qCritical() << "File with robots description has wrong format. Not enough items in the line.";
            break;
        }
        QString robotName = items[0];
        QString sPortNum = items[2];
        bool ok;
        int portNum = sPortNum.toInt(&ok);
        if (!ok) {
            result = FileLoadErrorWrongFormat;
            qCritical() << "File with robots description has wrong format. Port num is not integer.";
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
