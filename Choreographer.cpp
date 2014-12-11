#include "Choreographer.h"
#include <limits>
#include <QDebug>
#include "StopWatch.h"

void Choreographer::run()
{
    qDebug() << "Method name";

    dancing();
}

void Choreographer::dancing()
{
    qDebug() << "Method name";

    finished = false;
    paused = false;
    if (scenario->isMusicPlaying()) {
        musicPlayer = new MusicPlayer(scenario->getMusicFilePath());
        musicPlayer->start();
    }

    QVector<Role> roles = scenario->getRoles();
    QVector<DanceScript> scripts = scenario->getDanceScripts();

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
        for (int i = 0; i < roles.count(); ++i) {

            int danceNum = roles[i].danceNum;
            if (danceNum == -1) {
                continue;
            }

            if (scripts[danceNum].isFinished()) {
                continue;
            }

            int robotNum = roles[i].robotNum;
            if (scripts[danceNum].getCurrentFireTime()
                    <= timer->elapsedMilliseconds()) {
                Frame curFrame = scripts[danceNum].getCurrentFrame();
                robots[robotNum]->setPose(curFrame.servoAngles);
                scripts[danceNum].goToNextFrame();
            }

            haveWorkToDo |= !scripts[danceNum].isFinished();
        }

        finished |= !haveWorkToDo;
        if (finished) {
            break;
        }

        int closestFireTime = minFireTime(roles, scripts);
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

int Choreographer::minFireTime(QVector<Role> roles,
                               QVector<DanceScript> scripts)
{
    int result = std::numeric_limits<int>::max();
    for (int i = 0; i < roles.count(); ++i) {
        if (roles[i].danceNum != -1) {
            int time = scripts[roles[i].danceNum].getCurrentFireTime();
            result = std::min(result, time);
        }
    }

    return result;
}

Choreographer::Choreographer()
{
    qDebug() << "Method name";

    musicPlayer = NULL;
}

void Choreographer::setRobots(QVector<Robot*> t_robots)
{
    qDebug() << "Method name";

    robots = t_robots;
}

void Choreographer::setScenario(Scenario *t_scenario)
{
    qDebug() << "Method name";

    scenario = t_scenario;
}

void Choreographer::startDance()
{
    qDebug() << "Method name";

    run();
}

void Choreographer::pauseDance()
{
    qDebug() << "Method name";

    paused = !paused;
}

void Choreographer::stopDance()
{
    qDebug() << "Method name";

    finished = true;
}
