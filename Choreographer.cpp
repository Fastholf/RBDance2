#include "Choreographer.h"
#include <limits>
#include <QDebug>

void Choreographer::run()
{
    qDebug() << "Method name";

    qDebug() << "run: " << QThread::currentThread();
    dancing();
}

void Choreographer::dancing()
{
    qDebug() << "Method name";

    finished = false;
    paused = false;
    int currentTime = 0;
    if (scenario->isMusicPlaying()) {
        musicPlayer = new MusicPlayer(scenario->getMusicFilePath());
        musicPlayer->start();
    }

    QVector<Role> roles = scenario->getRoles();
    QVector<DanceScript> scripts = scenario->getDanceScripts();

    while (!finished) {

        while (paused) {
            QThread::msleep(1);
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
                    == currentTime) {
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
        while (currentTime < closestFireTime) {
            QThread::msleep(1);
            ++currentTime;
        }
    }

    if (musicPlayer != NULL) {
        musicPlayer->stop();
        delete musicPlayer;
        musicPlayer = NULL;
    }

    danceFinished();
}

int Choreographer::minFireTime(QVector<Role> roles,
                               QVector<DanceScript> scripts)
{
//    qDebug() << "Method name";

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

    qDebug() << "startDance: " << QThread::currentThread();
    run();
}

void Choreographer::pauseDance()
{
    qDebug() << "Method name";

    qDebug() << "pauseDance: " << QThread::currentThread();
    paused = !paused;
    if (musicPlayer != NULL) {
        musicPlayer->pause();
    }
}

void Choreographer::stopDance()
{
    qDebug() << "Method name";

    qDebug() << "stopDance: " << QThread::currentThread();
    finished = true;
}
