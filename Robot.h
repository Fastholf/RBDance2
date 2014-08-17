#ifndef ROBOT_H
#define ROBOT_H

#include <QString>
#include <QVector>
#include <QtSerialPort/QtSerialPort>
#include "RBController.h"

class Robot
{
private:
    int portNum;
    QString name;
    QSerialPort *serialPort;
    bool connected;
    bool DCModeOn;
    RBController *rbController;

public:
    Robot();
    Robot(int t_portNum, QString t_name);
    bool connect();
    void basicPosture();
    bool turnDCOn();
    void turnDCOff();
    void disconnect();

    bool isConnected();
    bool isDCModeOn();

    void setPose(QVector<int> servoAngles);
};

#endif // ROBOT_H
