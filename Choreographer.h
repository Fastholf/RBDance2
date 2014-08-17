#ifndef CHOREOGRAPHER_H
#define CHOREOGRAPHER_H

#include <QVector>
#include <QThread>
#include "Robot.h"
#include "Scenario.h"

class Choreographer : public QThread
{
    Q_OBJECT

protected:
    void run();

private:
    bool finished;
    bool paused;
    QVector<Robot> robots;
    Scenario *scenario;
    void dancing();

public:
    Choreographer();
    void startDance(QVector<Robot> t_robots, Scenario *t_scenario);
    void pauseDance();
    void stopDance();

signals:
    void danceFinished();
};

#endif // CHOREOGRAPHER_H
