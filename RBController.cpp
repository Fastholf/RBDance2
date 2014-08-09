#include "RBController.h"

RBController::RBController(QSerialPort *t_serialPort)
{
    serialPort = t_serialPort;
    header.append(0xFF);
    header.append(0xFF);
    header.append(0xAA);
    header.append(0x55);
    header.append(0xAA);
    header.append(0x55);
    header.append(0x37);
    header.append(0xBA);
}

void RBController::runBasicPosture()
{
    runMotion(7);
}

bool RBController::sendCommand(qint8 type, qint8 commandContents)
{
    QByteArray command;

    command.append(type);

    command.append((char)0x00); // platform

    command.append((char)0x00); // size
    command.append((char)0x00);
    command.append((char)0x00);
    command.append((char)0x01);

    command.append(commandContents);

    command.append(commandContents); // checkSum

    if (serialPort->write(header) != -1 &&
            serialPort->write(command) != -1) {
        return true;
    } else {
        qWarning() << "RBController::sendCommand: Can't write to port.";
    }

    return false;
}

void RBController::runMotion(qint8 motionNumber)
{
    sendCommand(20, motionNumber);
}
