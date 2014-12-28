#include "ScriptPlayer.h"
#include <QDebug>
#include <limits>

ScriptPlayer::ScriptPlayer(QVector<Robot*> t_robots, DanceScript t_script)
{
    robots = t_robots;
    script = t_script;
    reset();
}

void ScriptPlayer::setNextFrame()
{
    if (isFinished()) {
        return;
    }

    for (int i = 0; i < robots.count(); ++i) {
        robots[i]->setPose(script.getFrameAtIndex(curIndex).servoAngles);
    }
    ++curIndex;
}

bool ScriptPlayer::isFinished()
{
    return script.getFramesCount() <= curIndex;
}

int ScriptPlayer::getCurrentFireTime()
{
    if (isFinished()) {
        return std::numeric_limits<int>::max();
    }

    return script.getFireTimeAtIndex(curIndex);
}

int ScriptPlayer::getCurrentIndex()
{
    return curIndex;
}

void ScriptPlayer::goToNextIndex()
{
    ++curIndex;
}

void ScriptPlayer::reset()
{
    curIndex = 0;
}

void ScriptPlayer::setFrameAtIndex(int index)
{
    for (int i = 0; i < robots.count(); ++i) {
        robots[i]->setPose(script.getFrameAtIndex(index).servoAngles);
    }
}
