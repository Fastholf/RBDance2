#ifndef CHOREOGRAPHER_H
#define CHOREOGRAPHER_H

#include <QVector>
#include "Scenario.h"
#include "ScriptPlayer.h"
#include "MusicPlayer.h"

class PlayerState
{
public:
    int playerIndex;
    int frameIndex;
    PlayerState() {}
    PlayerState(int t_playerIndex, int t_frameIndex)
    {
        playerIndex = t_playerIndex;
        frameIndex = t_frameIndex;
    }
};

class DanceFrame
{
public:
    QVector<PlayerState*> playerStates;
    int fireTime;
    DanceFrame() {}
    DanceFrame(int t_fireTime, QVector<PlayerState*> t_playerStates)
    {
        fireTime = t_fireTime;
        playerStates = t_playerStates;
    }
};

class Choreographer : public QObject
{
    Q_OBJECT
private:
    Scenario *scenario;
    QVector<ScriptPlayer*> scriptPlayers;
    QVector<DanceFrame*> precountedDance;
    void precountDance();
    int curIndex;
public:
    Choreographer();
    void load(Scenario *t_scenario, QVector<Robot*> t_robots);
    void setNextFrame();
    int nextFireTime();
    bool shouldPlayMusic();
    QString getMusicFilePath();
    bool setPrecountedIndex(int index);
    void reset();
    bool isFinished();
signals:
    void danceLoaded(int maxIndex, int duration);
    void currentFrameChanged(int index, int elapsedTime);
};

#endif // CHOREOGRAPHER_H
