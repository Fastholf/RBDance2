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
    Scenario *scenario;
    QVector<ScriptPlayer*> scriptPlayers;
    MusicPlayer *musicPlayer;
    void dancing();
    int minFireTime();
    int longestScriptIndex;

public:
    Choreographer();
    void load(Scenario *t_scenario, QVector<Robot*> t_robots);
public slots:
    void startDance();
public:
    void pauseDance();
    void stopDance();
signals:
    void danceFinished();
    void danceLoaded(int maxIndex, int duration);
    void currentFrameChanged(int index, int elapsedTime);
};

#endif // CHOREOGRAPHER_H
