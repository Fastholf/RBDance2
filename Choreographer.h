#ifndef CHOREOGRAPHER_H
#define CHOREOGRAPHER_H

#include <QVector>
#include <QThread>
#include "Robot.h"
#include "Scenario.h"
#include "MusicPlayer.h"
#include "DanceScript.h"
#include "ScriptPlayer.h"

class Choreographer : public QObject
{
    Q_OBJECT

protected:
    void run();

private:
    bool finished;
    bool paused;
    QVector<Robot*> robots;
    Scenario *scenario;
    QVector<ScriptPlayer*> scriptPlayers;
    MusicPlayer *musicPlayer;
    void dancing();
    int minFireTime();
    int longestScriptIndex;

public:
    Choreographer();
    void setRobots(QVector<Robot*> t_robots);
    void setScenario(Scenario *t_scenario);
    void init();
public slots:
    void startDance();
public:
    void pauseDance();
    void stopDance();
signals:
    void danceFinished();
    void danceLoaded(int maxIndex);
    void currentFrameChanged(int index);
};

#endif // CHOREOGRAPHER_H
