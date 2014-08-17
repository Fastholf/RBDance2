#include "Choreographer.h"
#include <limits>
#include "DanceScript.h"
#include <QThread>

Choreographer::Choreographer()
{
}

void Choreographer::startDance(QVector<Robot> robots, Scenario scenario)
{
    finished = false;
    paused = false;
    int currentTime = 0;

    while (!finished)
    {
        while (paused) {

        }

        QVector<Role> roles = scenario.getRoles();
        int closestFireTime = scenario.minFireTime();
        bool hasWorkToDo = false;
        for (int i = 0; i < roles.count(); ++i)
        {
            int danceNum = roles[i].danceNum;
            if (danceNum == -1) {
                continue;
            }

            QVector<DanceScript> scripts = scenario.getDanceScripts();
            if (scripts[danceNum].isFinished()) {
                continue;
            }

            hasWorkToDo = true;

            int robotNum = roles[i].robotNum;
            if (scripts[danceNum].getCurrentFireTime()
                    == closestFireTime) {
                Frame curFrame = scripts[danceNum].getCurrentFrame();
                QVector<int> servoAngles = curFrame.servoAngles;
                robots[robotNum].setPose(servoAngles);
                scripts[danceNum].goToNextFrame();
            }
        }
        finished = hasWorkToDo;

        while (currentTime < closestFireTime) {
            QThread::msleep(1);
            ++currentTime;
        }
    }
}

void Choreographer::pauseDance()
{
    paused = !paused;
}

void Choreographer::stopDance()
{
    finished = true;
}
