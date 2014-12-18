#include "RBController.h"
#include "inttypes.h"

#define TURN_DC_ON_ATTEMPT_COUNT 6
#define WRITE_WAIT_TIMEOUT 10

RBController::RBController(int portNum)
{
//    qDebug() << "Method name";

    serialPort = new QSerialPort();
    _portNum = portNum;
    _connected = false;
    header.append(0xFF);
    header.append(0xFF);
    header.append(0xAA);
    header.append(0x55);
    header.append(0xAA);
    header.append(0x55);
    header.append(0x37);
    header.append(0xBA);
}

bool RBController::connectToRB()
{
//    qDebug() << "Method name";

//  For debug purpose only
//    _connected = true;
//    return true;

    if (_portNum == -1) {
        qWarning() << "Robot::connect: port num is not specified.";
        return false;
    }
    
    serialPort->setPortName("COM" + QString::number(_portNum));
    if (serialPort->open(QIODevice::ReadWrite)) {
        _connected = true;
        qDebug() << "Connection on port " << _portNum << " succeeded.";
        serialPort->setDataBits(QSerialPort::Data8);
    }
    else {
        _connected = false;
        qDebug() << "Connection on port " << _portNum << " failed.";
    }

    return _connected;
}

void RBController::runBasicPosture()
{
//    qDebug() << "Method name";

    runMotion(7);
}

bool RBController::turnDirectControlModeOn()
{
//    qDebug() << "Method name";

    if (!serialPort->isOpen()) {
        return false;
    }

    for (int i = 0; i < TURN_DC_ON_ATTEMPT_COUNT; ++i) {
        if (!sendCommand(0x10, 0x01)) {
            qWarning() << "Failed to send command on port.";
            return false;
        }

        // TODO: fix response reading.
        return true;

        QByteArray response;
        if (!getResponse(&response)) {
            qWarning() << "Failed to get response.";
            return false;
        }

        qDebug() << "Response:  " << response;
        if (response.at(14) == 0x01 && response.at(8) == 0x10) {
            return true;
        }
    }

    return false;
}

void RBController::turnDirectControlModeOff()
{
//    qDebug() << "Method name";

    if (!serialPort->isOpen()) {
        return;
    }

    QByteArray command;
    command.append((char)0xFF);
    command.append((char)0xE0);
    command.append((char)0xFB);
    command.append((char)0x1);
    command.append((char)0x00);
    command.append((char)0x1A);

    serialPort->write(command);
    if (serialPort->waitForBytesWritten(WRITE_WAIT_TIMEOUT)) {
        return;
    } else {
        qWarning() << "Can't write to port.";
    }
}

void RBController::disconnect()
{
//    qDebug() << "Method name";

    serialPort->close();
    _connected = false;
    qDebug() << "Connection on port " << _portNum << " closed.";
}

void RBController::setDirectPose(QVector<int> servoAngles)
{
//    qDebug() << "Method name";

//    For debug purpose only
//    return;

    int servoCount = servoAngles.count();
    QByteArray command;

    command.append((char)0xFF);                 //header;
    command.append((char)((4 << 5) | 0x1f));    // speed [0 ,4] | 31
    command.append(servoCount + 1);             // lastID + 1

    uint8_t checkSum = 0;
    for (int i = 0; i < servoCount; ++i)
    {
        command.append(servoAngles[i]);
        checkSum ^= (uint8_t)servoAngles[i];
    }
    checkSum = (uint8_t)(checkSum & 0x7f);
    command.append(checkSum);

    command.append((char)0x00);

    serialPort->write(command);
    if (serialPort->waitForBytesWritten(WRITE_WAIT_TIMEOUT)) {
//        qDebug() << "Pose set success.";
        return;
    }
    else {
        qWarning() << "Can't write to port.";
    }
}

bool RBController::sendCommand(qint8 type, qint8 commandContents)
{
//    qDebug() << "Method name";

    QByteArray command;

    command.append(type);

    command.append((char)0x00); // platform

    command.append((char)0x00); // size
    command.append((char)0x00);
    command.append((char)0x00);
    command.append((char)0x01);

    command.append(commandContents);

    command.append(commandContents); // checkSum

    qDebug() << "Command:  " << command;
    serialPort->write(header);
    bool writeSuccess = serialPort->waitForBytesWritten(WRITE_WAIT_TIMEOUT);
    serialPort->write(command);
    writeSuccess &= serialPort->waitForBytesWritten(WRITE_WAIT_TIMEOUT);
    if (writeSuccess) {
        return true;
    }
    else {
        qWarning() << "Can't write to port.";
    }

    return false;
}

bool RBController::getResponse(QByteArray *response)
{
//    qDebug() << "Method name";

    int b = 0, l = 1;
    while (b < 32 && b < (15 + l)) {

        char arr[2];
        qint8 readCount = serialPort->read(arr, 1);
        if (readCount == -1) {
            qWarning() << "Failed to read from port!";
            return false;
        }
        else if (readCount > 0) {
            response->append(arr[0]);
            response->append((char)0x01);
        }

        if (response->size() > 0 &&
                b < header.count() &&
                response->at(b) != (char)header.at(b)) {
            b = 0;
            response->clear();
            qDebug() << "Restart reading due to wrong byte in header";
            continue;
        }

        if (b == 13) {
            l = (response->at(b - 3) << 24) +
                (response->at(b - 2) << 16) +
                (response->at(b - 1) << 8) +
                 response->at(b);
        }

        ++b;

        if (readCount == 0 && !serialPort->waitForReadyRead(5000)) {
            qWarning() << "Stream is empty before response arrived.";
            return false;
        }
    }

    return true;
}

void RBController::runMotion(qint8 motionNumber)
{
//    qDebug() << "Method name";

    sendCommand(20, motionNumber);
}

RBController::RBController()
{
    qDebug() << "Method name";
}
