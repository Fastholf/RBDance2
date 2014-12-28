#include "ChoreographerWorker.h"
#include <limits>
#include <QDebug>
#include "StopWatch.h"

ChoreographerWorker::ChoreographerWorker(Choreographer *t_choreographer)
{
    choreographer = t_choreographer;
    musicPlayer = NULL;
}

void ChoreographerWorker::dancing()
{
//    qDebug() << "Method name";

    finished = false;
    paused = false;
    choreographer->reset();
    if (choreographer->shouldPlayMusic()) {
        musicPlayer = new MusicPlayer(choreographer->getMusicFilePath());
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

        choreographer->setNextFrame();
        if (choreographer->isFinished()) {
            break;
        }

        int nextFireTime = choreographer->nextFireTime();
        while (timer->elapsedMilliseconds() < nextFireTime) {
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

void ChoreographerWorker::startDance()
{
//    qDebug() << "Method name";

    dancing();
}

void ChoreographerWorker::pauseDance()
{
//    qDebug() << "Method name";

    paused = !paused;
}

void ChoreographerWorker::stopDance()
{
//    qDebug() << "Method name";

    finished = true;
}
