#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QDateTime>

class StopWatch : public QObject
{
    Q_OBJECT
private:
    int milliSecsElapsedOnPause;
    QDateTime startTime;
    QDateTime pausedTime;
public:
    typedef enum {
        STATE_STOPPED = 0,
        STATE_PAUSED,
        STATE_RUNNING,
    } State;
    State state;
    StopWatch();
    void start();
    void pause();
    int elapsedMilliseconds();
};

#endif // STOPWATCH_H
