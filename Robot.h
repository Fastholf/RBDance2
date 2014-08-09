#ifndef ROBOT_H
#define ROBOT_H

#include <QString>

class Robot
{
private:
    int portNum;
    QString name;

public:
    Robot();
    Robot(int t_portNum, QString t_name);
    void connect();
    void basicPosture();
    void turnDCOn();
    void turnDCOff();
    void disconnect();
};

#endif // ROBOT_H
