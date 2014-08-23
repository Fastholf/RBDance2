#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString.h>
#include <QVector>
#include <QObject>
#include "Robot.h"
#include "Scenario.h"
#include "Choreographer.h"

enum FileLoadError {
    FileLoadErrorNo = 0,
    FileLoadErrorNotFound,
    FileLoadErrorWrongFormat,
};

class AppManager : public QObject
{
    Q_OBJECT

private:
    QString robotsFilePath;
    QString scenarioListFilePath;
    QString settingsFilePath;
    QString rootPath;
    QVector<Robot> robots;
    QVector<QString> scenarioPaths;
    Scenario *scenario;
    Choreographer *choreographer;

    bool isRobotIndexOk(int index, QString methodName);

public:
    explicit AppManager();
    bool init();

    FileLoadError loadRobotsFromFile();
    FileLoadError loadScenarioListFromFile();
    FileLoadError loadScenarioFromFile(int scenarioIndex);

    void setRobotRole(int robotNum, int danceNum);

    bool connectRobot(int index);
    void robotBasicPosture(int index);
    bool robotTurnDCOn(int index);
    void robotTurnDCOff(int index);
    void robotDisconnect(int index);
    bool isDanceReady();
    void danceStart();
    void dancePause();
    void danceStop();

private slots:
    void onDanceFinished();

signals:
    void robotLoaded(int index, QString name, int port);
    void scenarioListLoaded(QVector<QString> scenarioPaths);
    void scenarioLoaded(QVector<QString> danceFileNames, QVector<Role> roles);
    void danceFinished();
    void showMessage(QString message);
};

#endif // CONTROLLER_H
