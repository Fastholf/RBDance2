#include "Choreographer.h"

Choreographer::Choreographer()
{
    scenario = NULL;
    longestScriptIndex = 0;
}

void Choreographer::load(Scenario *t_scenario, QVector<Robot *> t_robots)
{
    scenario = t_scenario;
    QVector<Robot*> robots = t_robots;
    QVector<Role> roles = scenario->getRoles();
    QVector<DanceScript> scripts = scenario->getDanceScripts();
    scriptPlayers.clear();
    longestScriptIndex = -1;
    for (int i = 0; i < roles.count(); ++i) {
        if (roles[i].danceNum != -1) {
            int danceNum = roles[i].danceNum;
            QVector<Robot*> t_robots;
            DanceScript script = scripts[roles[i].danceNum];
            for (int j = i; j < roles.count(); ++j) {
                if (danceNum == roles[j].danceNum) {
                    t_robots.push_back(robots[roles[j].robotNum]);
                    roles[j].danceNum = -1;
                }
            }
            scriptPlayers.push_back(new ScriptPlayer(t_robots, script));
        }
    }
    int longestTime = 0;
    for (int i = 0; i < scriptPlayers.count(); ++i) {
        if (scriptPlayers[i]->getTimeLength() > longestTime) {
            longestScriptIndex = i;
            longestTime = scriptPlayers[i]->getTimeLength();
        }
    }
    int maxIndex = 0;
    if (longestScriptIndex != -1) {
        maxIndex = scriptPlayers[longestScriptIndex]->getMaxIndex();
    }
    emit danceLoaded(maxIndex, longestTime);
}

bool Choreographer::setNextFrame(int elapsedMilliseconds)
{
    bool haveWorkToDo = false;
    for (int i = 0; i < scriptPlayers.count(); ++i) {
        if (scriptPlayers[i]->isFinished()) {
            continue;
        }

        if (scriptPlayers[i]->getCurrentFireTime() <= elapsedMilliseconds) {
            scriptPlayers[i]->setNextFrame();
            if (i == longestScriptIndex) {
                int curIndex = scriptPlayers[i]->getCurrentIndex();
                emit currentFrameChanged(curIndex, elapsedMilliseconds);
            }
        }

        haveWorkToDo |= !scriptPlayers[i]->isFinished();
    }

    return !haveWorkToDo;
}

int Choreographer::nextFireTime()
{
    int result = std::numeric_limits<int>::max();
    for (int i = 0; i < scriptPlayers.count(); ++i) {
        int time = scriptPlayers[i]->getCurrentFireTime();
        result = std::min(result, time);
    }

    return result;
}

bool Choreographer::shouldPlayMusic()
{
    return scenario->isMusicPlaying();
}

QString Choreographer::getMusicFilePath()
{
    return scenario->getMusicFilePath();
}
