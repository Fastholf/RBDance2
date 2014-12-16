#ifndef SCRIPTPLAYER_H
#define SCRIPTPLAYER_H

#include <QVector>
#include "Robot.h"
#include "DanceScript.h"

class ScriptPlayer
{
private:
    QVector<Robot*> robots;
    DanceScript script;
public:
    ScriptPlayer(QVector<Robot*> t_robots, DanceScript t_script);
    void setNextFrame();
    bool isFinished();
    int getCurrentFireTime();
};

#endif // SCRIPTPLAYER_H
