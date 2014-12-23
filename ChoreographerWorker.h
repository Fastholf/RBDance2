#ifndef CHOREOGRAPHER_WORKER_H
#define CHOREOGRAPHER_WORKER_H

#include <QVector>
#include <QThread>
#include "Robot.h"
#include "Scenario.h"
#include "MusicPlayer.h"
#include "DanceScript.h"
#include "ScriptPlayer.h"
#include "Choreographer.h"

class ChoreographerWorker : public QObject
{
    Q_OBJECT

private:
    MusicPlayer *musicPlayer;
    bool finished;
    bool paused;
    void dancing();
    Choreographer *choreographer;

public:
    ChoreographerWorker(Choreographer *t_choreographer);
public slots:
    void startDance();
public:
    void pauseDance();
    void stopDance();
signals:
    void danceFinished();
};

#endif // CHOREOGRAPHER_WORKER_H
