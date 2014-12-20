#include "Choreographer.h"
#include <limits>
#include <QDebug>
#include "StopWatch.h"

void Choreographer::run()
{
//    qDebug() << "Method name";

    dancing();
}

void Choreographer::dancing()
{
//    qDebug() << "Method name";

    finished = false;
    paused = false;
    if (scenario->isMusicPlaying()) {
        musicPlayer = new MusicPlayer(scenario->getMusicFilePath());
        musicPlayer->start();
    }

    StopWatch *timer = new StopWatch();
    timer->start();

    while (!finished) {

        if (paused) {
            if (musicPlayer != NULL) {
                musicPlayer->pause();
            }
            timer->pause();
        }
        while (paused) {
            QThread::msleep(1);
        }
        if (timer->state == StopWatch::STATE_PAUSED) {
            timer->pause();
        }
        if (musicPlayer != NULL) {
            if (musicPlayer->isPaused()) {
                musicPlayer->pause();
            }
        }

//        For debug purpose only
//        if (finished) {
//            break;
//        }
//        QThread::msleep(1);
//        continue;

        bool haveWorkToDo = false;
        int elapsedMilliseconds = timer->elapsedMilliseconds();
        for (int i = 0; i < scriptPlayers.count(); ++i) {
            if (scriptPlayers[i]->isFinished()) {
                continue;
            }

            if (scriptPlayers[i]->getCurrentFireTime() <= elapsedMilliseconds) {
                scriptPlayers[i]->setNextFrame();
                if (i == longestScriptIndex) {
                    emit currentFrameChanged(scriptPlayers[i]->getCurrentIndex());
                }
            }

            haveWorkToDo |= !scriptPlayers[i]->isFinished();
        }

        finished |= !haveWorkToDo;
        if (finished) {
            break;
        }

        int closestFireTime = minFireTime();
        while (timer->elapsedMilliseconds() < closestFireTime) {
            QThread::msleep(1);
        }
    }

    if (musicPlayer != NULL) {
        musicPlayer->stop();
        delete musicPlayer;
        musicPlayer = NULL;
    }

    delete timer;

    danceFinished();
}

int Choreographer::minFireTime()
{
    int result = std::numeric_limits<int>::max();
    for (int i = 0; i < scriptPlayers.count(); ++i) {
        int time = scriptPlayers[i]->getCurrentFireTime();
        result = std::min(result, time);
    }

    return result;
}

Choreographer::Choreographer()
{
//    qDebug() << "Method name";

    musicPlayer = NULL;
    longestScriptIndex = 0;
}

void Choreographer::setRobots(QVector<Robot*> t_robots)
{
//    qDebug() << "Method name";

    robots = t_robots;
}

void Choreographer::setScenario(Scenario *t_scenario)
{
//    qDebug() << "Method name";

    scenario = t_scenario;
}

void Choreographer::init()
{
    QVector<Role> roles = scenario->getRoles();
    QVector<DanceScript> scripts = scenario->getDanceScripts();
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
    emit danceLoaded(scriptPlayers[longestScriptIndex]->getMaxIndex());
}

void Choreographer::startDance()
{
//    qDebug() << "Method name";

    run();
}

void Choreographer::pauseDance()
{
//    qDebug() << "Method name";

    paused = !paused;
}

void Choreographer::stopDance()
{
//    qDebug() << "Method name";

    finished = true;
}
