#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString.h>
#include <QVector>
#include <QObject>
#include "Robot.h"
#include "Scenario.h"

enum FileLoadError {
    FileLoadErrorNo = 0,
    FileLoadErrorNotFound,
    FileLoadErrorWrongFormat,
};

class Controller : public QObject
{
    Q_OBJECT

private:
    QString robotsFileName;
    QString scenarioListFileName;
    QVector<Robot> robots;
    QVector<QString> scenarioPaths;
    Scenario *scenario;

    bool isRobotIndexOk(int index, QString methodName);

public:
    explicit Controller();

    FileLoadError loadRobotsFromFile();
    FileLoadError loadScenarioListFromFile();
    FileLoadError loadScenarioFromFile(int scenarioIndex);

    void setRobotRole(int robotNum, int danceNum);

    void connectRobot(int index);
    void robotBasicPosture(int index);
    void robotTurnDCOn(int index);
    void robotTurnDCOff(int index);
    void robotDisconnect(int index);
    bool isDanceReady();
    void danceStart();
    void dancePause();
    void danceStop();

signals:
    void robotLoaded(int index, QString name, int port);
    void scenarioListLoaded(QVector<QString> scenarioPaths);
    void scenarioLoaded(QVector<QString> danceFileNames, QVector<Role> roles);

};

#endif // CONTROLLER_H
