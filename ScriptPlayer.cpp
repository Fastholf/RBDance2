#include "ScriptPlayer.h"
#include <QDebug>

ScriptPlayer::ScriptPlayer(QVector<Robot*> t_robots, DanceScript t_script)
{
    robots = t_robots;
    script = t_script;
}

void ScriptPlayer::setNextFrame()
{
    if (script.isFinished()) {
        return;
    }

    for (int i = 0; i < robots.count(); ++i) {
        robots[i]->setPose(script.getCurrentFrame().servoAngles);
    }
    script.goToNextFrame();
}

bool ScriptPlayer::isFinished()
{
    return script.isFinished();
}

int ScriptPlayer::getCurrentFireTime()
{
    return script.getCurrentFireTime();
}

int ScriptPlayer::getTimeLength()
{
    return script.getTimeLength();
}

int ScriptPlayer::getCurrentIndex()
{
    return script.getCurrentIndex();
}

int ScriptPlayer::getMaxIndex()
{
    return script.getMaxIndex();
}
