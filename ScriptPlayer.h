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
    int curIndex;
public:
    ScriptPlayer(QVector<Robot*> t_robots, DanceScript t_script);
    void setNextFrame();
    bool isFinished();
    int getCurrentFireTime();
    int getCurrentIndex();
    void goToNextIndex();
    void reset();
    void setFrameAtIndex(int index);
};

#endif // SCRIPTPLAYER_H
