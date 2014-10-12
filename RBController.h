#ifndef RBCONTROLLER_H
#define RBCONTROLLER_H

#include <QtSerialPort/QtSerialPort>
#include <QVector>

class RBController
{
    QSerialPort *serialPort;
    int _portNum;
    QByteArray header;
    bool _connected;
    bool sendCommand(qint8 type, qint8 commandContents);
    bool getResponse(QByteArray *response);
    void runMotion(qint8 motionNumber);

public:
    RBController();
    RBController(int portNum);
    bool connectToRB();
    void runBasicPosture();
    bool turnDirectControlModeOn();
    void setDirectPose(QVector<int> servoAngles);
    void turnDirectControlModeOff();
    void disconnect();
};

#endif // RBCONTROLLER_H
