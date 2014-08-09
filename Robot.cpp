#include "Robot.h"

Robot::Robot()
{
    portNum = -1;
    connected = false;
}

Robot::Robot(int t_portNum, QString t_name)
{
    portNum = t_portNum;
    name = t_name;
    connected = false;
}

void Robot::connect()
{
    if (portNum == -1) {
        qWarning() << "Robot::connect: port num is not specified.";
        return;
    }

    serialPort = new QSerialPort();
    serialPort->setPortName("COM" + QString::number(portNum));
    if (serialPort->open(QIODevice::ReadWrite)) {
        connected = true;
        rbController = new RBController(serialPort);
        qDebug() << "Robot " << name << " connected.";
    }
    else {
        qDebug() << "Connection on port " << portNum << " failed.";
    }
}

void Robot::basicPosture()
{
    if (isConnected()) {
        rbController->runBasicPosture();
    }
}

void Robot::turnDCOn()
{

}

void Robot::turnDCOff()
{

}

void Robot::disconnect()
{
    serialPort->close();
    connected = false;
}

bool Robot::isConnected()
{
    return connected;
}
