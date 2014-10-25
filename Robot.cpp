#include "Robot.h"

Robot::Robot()
{
    qDebug() << "Method name";

    portNum = -1;
    _connected = false;
    _DCModeOn = false;
}

Robot::Robot(int index,
             int t_portNum,
             QString t_name,
             SerialPortWorker *portWorker)
{
    qDebug() << "Method name";

    _index = index;
    portNum = t_portNum;
    name = t_name;

    _portWorker = portWorker;
    connect(_portWorker, SIGNAL(connectTryFinished(int, bool)),
            this, SLOT(onConnectTryFinished(int, bool)));
    connect(_portWorker, SIGNAL(turnDCOnFinished(int,bool)),
            this, SLOT(onTurnDCOnFinished(int,bool)));
    connect(_portWorker, SIGNAL(turnDCOffFinished(int)),
            this, SLOT(onTurnDCOffFinished(int)));
    connect(_portWorker, SIGNAL(disconnected(int)),
            this, SLOT(onDisconnected(int)));

    _connected = false;
    _DCModeOn = false;
}

QString Robot::getName()
{
//    qDebug() << "Method name";

    return name;
}

void Robot::connectToRB()
{
    qDebug() << "Method name";

    _portWorker->connectToRB(_index);
}

void Robot::basicPosture()
{
    qDebug() << "Method name";

    _portWorker->basicPosture(_index);
}

void Robot::turnDCOn()
{
    qDebug() << "Method name";

    _portWorker->turnDCOn(_index);
}

void Robot::setPose(QVector<int> servoAngles)
{
//    qDebug() << "Method name";

    _portWorker->setPose(_index, servoAngles);
}

void Robot::turnDCOff()
{
    qDebug() << "Method name";

    _portWorker->turnDCOff(_index);
}

void Robot::disconnect()
{
    qDebug() << "Method name";

    _portWorker->disconnect(_index);
}

void Robot::onConnectTryFinished(int index, bool connected)
{
    qDebug() << "Method name";

    if (index != _index) {
        return;
    }
    
    _connected = connected;
    emit connectTryFinished(index, connected);
}

void Robot::onTurnDCOnFinished(int index, bool result)
{
    qDebug() << "Method name";

    if (index != _index) {
        return;
    }

    _DCModeOn = result;
    emit turnDCOnFinished(index, result);
}

void Robot::onTurnDCOffFinished(int index)
{
    qDebug() << "Method name";

    if (index != _index) {
        return;
    }

    _DCModeOn = false;
    emit turnDCOffFinished(index);
}

void Robot::onDisconnected(int index)
{
    qDebug() << "Method name";

    if (index != _index) {
        return;
    }

    emit disconnected(index);
}

bool Robot::isConnected()
{
//    qDebug() << "Method name";

    return _connected;
}

bool Robot::isDCModeOn()
{
//    qDebug() << "Method name";

    return _DCModeOn;
}
