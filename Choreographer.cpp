#include "Choreographer.h"

Choreographer::Choreographer()
{
    scenario = NULL;
    longestScriptIndex = 0;
}

void Choreographer::load(Scenario *t_scenario, QVector<Robot *> t_robots)
{
    qDebug() << "Method name";

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
    precountDance();
    int maxIndex = 0;
    int duration = 0;
    if (precountedDance.count() > 0) {
        maxIndex = precountedDance.count() - 1;
        duration = precountedDance[precountedDance.count() - 1]->fireTime;
    }
    emit danceLoaded(maxIndex, duration);
}

void Choreographer::precountDance()
{
    qDebug() << "Method name";

    precountedDance.clear();
    while (true) {
        bool notFinished = false;
        int nextFireTime = std::numeric_limits<int>::max();
        for (int i = 0; i < scriptPlayers.count(); ++i) {
            if (scriptPlayers[i]->isFinished()) {
                continue;
            }
            notFinished = true;
            int time = scriptPlayers[i]->getCurrentFireTime();
            nextFireTime = std::min(nextFireTime, time);
        }
        if (!notFinished) {
            break;
        }
        QVector<PlayerState*> playerStates;
        for (int i = 0; i < scriptPlayers.count(); ++i) {
            int time = scriptPlayers[i]->getCurrentFireTime();
            if (nextFireTime == time) {
                int index = scriptPlayers[i]->getCurrentIndex();
                playerStates.push_back(new PlayerState(i, index));
                scriptPlayers[i]->goToNextIndex();
            }
        }
        precountedDance.push_back(new DanceFrame(nextFireTime, playerStates));
    }
    for (int i = 0; i < scriptPlayers.count(); ++i) {
        scriptPlayers[i]->reset();
    }
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

bool Choreographer::setPrecountedIndex(int index)
{
    if (index >= precountedDance.count()) {
        return false;
    }

    int fireTime = precountedDance[index]->fireTime;
    QVector<PlayerState*> playerStates = precountedDance[index]->playerStates;
    for (int i = 0; i < playerStates.count(); ++i) {
        int playerIndex = playerStates[i]->playerIndex;
        int frameIndex = playerStates[i]->frameIndex;
        scriptPlayers[playerIndex]->setFrameAtIndex(frameIndex);
    }
    emit currentFrameChanged(index, fireTime);
    return true;
}
