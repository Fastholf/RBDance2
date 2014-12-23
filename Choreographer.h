#ifndef CHOREOGRAPHER_H
#define CHOREOGRAPHER_H

#include <QVector>
#include "Scenario.h"
#include "ScriptPlayer.h"
#include "MusicPlayer.h"

class Choreographer : public QObject
{
    Q_OBJECT
private:
    Scenario *scenario;
    QVector<ScriptPlayer*> scriptPlayers;
    int longestScriptIndex;
public:
    Choreographer();
    void load(Scenario *t_scenario, QVector<Robot*> t_robots);
    bool setNextFrame(int elapsedMilliseconds);
    int nextFireTime();
    bool shouldPlayMusic();
    QString getMusicFilePath();
signals:
    void danceLoaded(int maxIndex, int duration);
    void currentFrameChanged(int index, int elapsedTime);
};

#endif // CHOREOGRAPHER_H
