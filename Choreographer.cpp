#include "Choreographer.h"
#include <limits>
#include "DanceScript.h"
#include <QDebug>

void Choreographer::run()
{
    dancing();
    musicPlayer = NULL;
}

void Choreographer::dancing()
{
    finished = false;
    paused = false;
    int currentTime = 0;
    if (scenario->isMusicPlaying()) {
        musicPlayer = new MusicPlayer(scenario->getMusicFilePath());
        musicPlayer->start();
    }

    while (!finished) {

        while (paused) {
            QThread::msleep(1);
        }

        QVector<Role> roles = scenario->getRoles();
        bool haveWorkToDo = false;
        for (int i = 0; i < roles.count(); ++i) {

            int danceNum = roles[i].danceNum;
            if (danceNum == -1) {
                continue;
            }

            QVector<DanceScript> scripts = scenario->getDanceScripts();
            if (scripts[danceNum].isFinished()) {
                continue;
            }

            int robotNum = roles[i].robotNum;
            if (scripts[danceNum].getCurrentFireTime()
                    == currentTime) {
                Frame curFrame = scripts[danceNum].getCurrentFrame();
                robots[robotNum].setPose(curFrame.servoAngles);
                scripts[danceNum].goToNextFrame();
            }

            haveWorkToDo |= scripts[danceNum].isFinished();
        }
        finished |= !haveWorkToDo;
        if (finished) {
            break;
        }

        int closestFireTime = scenario->minFireTime();
        while (currentTime < closestFireTime) {
            QThread::msleep(1);
            ++currentTime;
        }
    }
    danceFinished();
}

Choreographer::Choreographer()
{
}

void Choreographer::startDance(QVector<Robot> t_robots, Scenario *t_scenario)
{
    robots = t_robots;
    scenario = t_scenario;
    start();
}

void Choreographer::pauseDance()
{
    paused = !paused;
    if (musicPlayer != NULL) {
        musicPlayer->pause();
    }
}

void Choreographer::stopDance()
{
    finished = true;
    if (musicPlayer != NULL) {
        musicPlayer->stop();
        delete musicPlayer;
        musicPlayer = NULL;
    }
}
