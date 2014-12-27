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
    int getTimeLength();
    int getCurrentIndex();
    int getMaxIndex();
    void goToNextIndex();
    void reset();
    void setFrameAtIndex(int index);
};

#endif // SCRIPTPLAYER_H
