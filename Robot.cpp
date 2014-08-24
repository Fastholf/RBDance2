#include "Robot.h"

Robot::Robot()
{
    portNum = -1;
    connected = false;
    DCModeOn = false;
}

Robot::Robot(int t_portNum, QString t_name)
{
    portNum = t_portNum;
    name = t_name;
    connected = false;
    DCModeOn = false;
}

QString Robot::getName()
{
    return name;
}

bool Robot::connect()
{
    if (portNum == -1) {
        qWarning() << "Robot::connect: port num is not specified.";
        return false;
    }

    serialPort = new QSerialPort();
    serialPort->setPortName("COM" + QString::number(portNum));
    serialPort->setDataBits(QSerialPort::Data8);
    if (serialPort->open(QIODevice::ReadWrite)) {
        connected = true;
        rbController = new RBController(serialPort);
        qDebug() << "Robot " << name << " connected.";
        return true;
    }
    else {
        qDebug() << "Connection on port " << portNum << " failed.";
        return false;
    }
}

void Robot::basicPosture()
{
    if (isConnected() && !isDCModeOn()) {
        rbController->runBasicPosture();
    }
}

bool Robot::turnDCOn()
{
    if (isConnected()) {
        DCModeOn = rbController->turnDirectControlModeOn(10);
    }

    return DCModeOn;
}

void Robot::turnDCOff()
{
    if (isConnected()) {
        rbController->turnDirectControlModeOff();
        DCModeOn = false;
    }
}

void Robot::disconnect()
{
    if (isConnected()) {
        serialPort->close();
        connected = false;
        qDebug() << "Robot " << name << " disconnected.";
    }
}

bool Robot::isConnected()
{
    return connected;
}

bool Robot::isDCModeOn()
{
    return DCModeOn;
}

void Robot::setPose(QVector<int> servoAngles)
{
    rbController->setDirectPose(servoAngles);
}
