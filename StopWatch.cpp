#include "StopWatch.h"

StopWatch::StopWatch()
{
    state = STATE_STOPPED;
    milliSecsElapsedOnPause = 0;
}

void StopWatch::start()
{
    startTime = QDateTime::currentDateTime();
    state = STATE_RUNNING;
}

void StopWatch::pause()
{
    if (state == STATE_STOPPED) {
        return;
    }

    if (state == STATE_RUNNING) {
        state = STATE_PAUSED;
        pausedTime = QDateTime::currentDateTime();
    }
    else if (state == STATE_PAUSED) {
        state = STATE_RUNNING;
        QDateTime unpausedTime = QDateTime::currentDateTime();
        int elapsedMilliseconds = pausedTime.msecsTo(unpausedTime);
        milliSecsElapsedOnPause += elapsedMilliseconds;
    }
}

int StopWatch::elapsedMilliseconds()
{
    if (state == STATE_STOPPED) {
        return 0;
    }

    QDateTime currentTime = QDateTime::currentDateTime();
    return startTime.msecsTo(currentTime) - milliSecsElapsedOnPause;
}

