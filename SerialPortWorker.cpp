#include "SerialPortWorker.h"
#include <QDebug>

void SerialPortWorker::processCommand(Command command)
{
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
    RBController *rbController = new RBController(portNum);
    rbControllers.push_back(*rbController);
}

void SerialPortWorker::processConnect(int index)
{
    bool connected = rbControllers[index].connectToRB();
    emit connectTryFinished(index, connected);
}

void SerialPortWorker::processBasicPosture(int index)
{
    rbControllers[index].runBasicPosture();
    emit basicPostureFinished(index);
}

void SerialPortWorker::processDCOn(int index)
{
    bool result = rbControllers[index].turnDirectControlModeOn();
    emit turnDCOnFinished(index, result);
}

void SerialPortWorker::processSetPose(int index, QVector<int> servoAngles)
{
    rbControllers[index].setDirectPose(servoAngles);
}

void SerialPortWorker::processDCOff(int index)
{
    rbControllers[index].turnDirectControlModeOff();
    emit turnDCOffFinished(index);
}

void SerialPortWorker::processDisconnect(int index)
{
    rbControllers[index].disconnect();
    emit disconnected(index);
}

SerialPortWorker::SerialPortWorker()
{
    working = true;
}

void SerialPortWorker::addSerialPort(int portNum)
{
    Command command(CT_create, portNum);
    workingQueue.enqueue(command);
}

void SerialPortWorker::connectToRB(int index)
{
    Command command(CT_connect, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::basicPosture(int index)
{
    Command command(CT_basicPosture, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::turnDCOn(int index)
{
    Command command(CT_DCOn, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::setPose(int index, QVector<int> servoAngles)
{
    Command command(CT_setPose, index, servoAngles);
    workingQueue.enqueue(command);
}

void SerialPortWorker::turnDCOff(int index)
{
    Command command(CT_DCOff, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::disconnect(int index)
{
    Command command(CT_disconnect, index);
    workingQueue.enqueue(command);
}

void SerialPortWorker::runQueue()
{
    while (working) {
        if (!workingQueue.empty()) {
            Command command = workingQueue.dequeue();
            processCommand(command);
        }
    }
    emit workFinished();
}
