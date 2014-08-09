#ifndef ROBOT_H
#define ROBOT_H

#include <QString>
#include <QtSerialPort/QtSerialPort>
#include "RBController.h"

class Robot
{
private:
    int portNum;
    QString name;
    QSerialPort *serialPort;
    bool connected;
    RBController *rbController;

public:
    Robot();
    Robot(int t_portNum, QString t_name);
    void connect();
    void basicPosture();
    void turnDCOn();
    void turnDCOff();
    void disconnect();

    bool isConnected();
};

#endif // ROBOT_H
