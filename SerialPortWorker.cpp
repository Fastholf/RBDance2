#include "SerialPortWorker.h"
#include <QDebug>

void SerialPortWorker::processCommand(Command command)
{
//    qDebug() << "Method name";

    switch (command.type) {

    case CT_create:
        processCreate(command.num);
        break;

    case CT_connect:
        processConnect(command.num);
        break;

    case CT_basicPosture:
        processBasicPosture(command.num);
        break;

    case CT_DCOn:
        processDCOn(command.num);
        break;

    case CT_setPose:
        processSetPose(command.num, command.angles);
        break;

    case CT_DCOff:
        processDCOff(command.num);
        break;

    case CT_disconnect:
        processDisconnect(command.num);
        break;

//    default:
//        break;

    }
}

void SerialPortWorker::processCreate(int portNum)
{
    qDebug() << "Method name";

    RBController *rbController = new RBController(portNum);
    rbControllers.push_back(*rbController);
}

void SerialPortWorker::processConnect(int index)
{
    qDebug() << "Method name";

    bool connected = rbControllers[index].connectToRB();
    emit connectTryFinished(index, connected);
}

void SerialPortWorker::processBasicPosture(int index)
{
    qDebug() << "Method name";

    rbControllers[index].runBasicPosture();
    emit basicPostureFinished(index);
}

void SerialPortWorker::processDCOn(int index)
{
    qDebug() << "Method name";

    bool result = rbControllers[index].turnDirectControlModeOn();
    emit turnDCOnFinished(index, result);
}

void SerialPortWorker::processSetPose(int index, QVector<int> servoAngles)
{
//    qDebug() << "Method name";

    rbControllers[index].setDirectPose(servoAngles);
}

void SerialPortWorker::processDCOff(int index)
{
    qDebug() << "Method name";

    rbControllers[index].turnDirectControlModeOff();
    emit turnDCOffFinished(index);
}

void SerialPortWorker::processDisconnect(int index)
{
    qDebug() << "Method name";

    rbControllers[index].disconnect();
    emit disconnected(index);
}

SerialPortWorker::SerialPortWorker()
{
    qDebug() << "Method name";

    working = true;
}

void SerialPortWorker::addSerialPort(int portNum)
{
    qDebug() << "Method name";

    Command command(CT_create, portNum);
    workingQueue.enqueue(command);
}

void SerialPortWorker::connectToRB(int index)
{
    qDebug() << "Method name";

    Command command(CT_connect, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::basicPosture(int index)
{
    qDebug() << "Method name";

    Command command(CT_basicPosture, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::turnDCOn(int index)
{
    qDebug() << "Method name";

    Command command(CT_DCOn, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::setPose(int index, QVector<int> servoAngles)
{
//    qDebug() << "Method name";

    Command command(CT_setPose, index, servoAngles);
    workingQueue.enqueue(command);
}

void SerialPortWorker::turnDCOff(int index)
{
    qDebug() << "Method name";

    Command command(CT_DCOff, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::disconnect(int index)
{
    qDebug() << "Method name";

    Command command(CT_disconnect, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::runQueue()
{
    qDebug() << "Method name";

    while (working) {
        if (!workingQueue.empty()) {
            Command command = workingQueue.dequeue();
            processCommand(command);
        }
        else {
            QThread::msleep(1);
        }
    }
    emit workFinished();
}
