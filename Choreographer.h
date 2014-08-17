#ifndef CHOREOGRAPHER_H
#define CHOREOGRAPHER_H

#include <QVector>
#include "Robot.h"
#include "Scenario.h"

class Choreographer
{
private:
    bool finished;
    bool paused;

public:
    Choreographer();
    void startDance(QVector<Robot> robots, Scenario scenario);
    void pauseDance();
    void stopDance();
};

#endif // CHOREOGRAPHER_H
