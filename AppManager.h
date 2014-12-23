#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString.h>
#include <QVector>
#include <QObject>
#include "Robot.h"
#include "Scenario.h"
#include "Choreographer.h"
#include "ChoreographerWorker.h"
#include "SerialPortWorker.h"
#include "FileLoadError.h"

class AppManager : public QObject
{
    Q_OBJECT

private:
    QString robotsFilePath;
    QString scenarioListFilePath;
    QString settingsFilePath;
    QString rootPath;
    QVector<Robot*> robots;
    QVector<QString> scenarioPaths;
    bool musicPlaying;
    Scenario *scenario;
    ChoreographerWorker *choreographerWorker;
    Choreographer *choreographer;
    SerialPortWorker *serialPortWorker;
    QThread *serialPortThread;
    QThread *choreographerThread;

    bool isRobotIndexOk(int index, QString methodName);

public:
    explicit AppManager();
    bool init();

    FileLoadError loadRobotsFromFile();
    FileLoadError loadScenarioListFromFile();
    FileLoadError loadScenarioFromFile(int scenarioIndex);
    void resetScenario();

    void setRobotRole(int robotNum, int danceNum);
    void setMusicPlaying(bool t_musicPlaying);

    void connectRobot(int index);
    void connectAllRobots();
    void robotBasicPosture(int index);
    void robotBasicPostureAll();
    void robotTurnDCOn(int index);
    void robotTurnDCOnAll();
    void robotTurnDCOff(int index);
    void robotTurnDCOffAll();
    void robotDisconnect(int index);
    void robotDisconnectAll();
    bool isDanceReady();
    void danceStart();
    void dancePause();
    void danceStop();

private slots:
    void onDanceFinished();
    void onConnectTryFinished(int index, bool result);
    void onTurnDCOnFinished(int index, bool result);
    void onTurnDCOffFinished(int index);
    void onDisconnected(int index);
    void onDanceLoaded(int maxIndex, int duration);
    void onCurrentFrameChanged(int index, int elapsedTime);

signals:
    void robotLoaded(int index, QString name, int port);
    void scenarioListLoaded(QVector<QString> scenarioPaths);
    void scenarioLoaded(QVector<QString> danceFileNames, QVector<Role> roles);
    void danceFinished();
    void showMessage(QString message);
    void connectTryFinished(int index, bool result);
    void turnDCOnFinished(int index, bool result);
    void turnDCOffFinished(int index);
    void disconnected(int index);
    void danceLoaded(int maxIndex, int duration);
    void currentFrameChanged(int index, int elapsedTime);
};

#endif // CONTROLLER_H
