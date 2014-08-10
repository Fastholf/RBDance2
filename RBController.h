#ifndef RBCONTROLLER_H
#define RBCONTROLLER_H

#include <QtSerialPort/QtSerialPort>

class RBController
{
    QSerialPort *serialPort;
    QByteArray header;
    bool sendCommand(qint8 type, qint8 commandContents);
    bool getResponse(QByteArray *response);
    void runMotion(qint8 motionNumber);

public:
    RBController(QSerialPort *t_serialPort);
    void runBasicPosture();
    bool turnDirectControlModeOn(int attemptCount);
    void turnDirectControlModeOff();
};

#endif // RBCONTROLLER_H
