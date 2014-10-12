#ifndef SERIALPORTWORKER_H
#define SERIALPORTWORKER_H

#include <QVector>
#include <QQueue>
#include "RBController.h"

typedef enum {
    CT_create,
    CT_connect,
    CT_basicPosture,
    CT_DCOn,
    CT_setPose,
    CT_DCOff,
    CT_disconnect,
} CommandType;

class Command
{
public:
    CommandType type;
    int num;
    QVector<int> angles;

    Command();
    Command(CommandType t_type, int t_num)
        :type(t_type), num(t_num) {}
    Command(CommandType t_type, int t_num, QVector<int> t_angles)
        :type(t_type), num(t_num), angles(t_angles) {}
};

class SerialPortWorker : public QObject
{
    Q_OBJECT

private:
    QVector<RBController> rbControllers;
    QQueue<Command> workingQueue;
    bool working;

    void processCommand(Command command);
    void processCreate(int portNum);
    void processConnect(int index);
    void processBasicPosture(int index);
    void processDCOn(int index);
    void processSetPose(int index, QVector<int> servoAngles);
    void processDCOff(int index);
    void processDisconnect(int index);

public:
    SerialPortWorker();
    void addSerialPort(int portNum);
    void connectToRB(int index);
    void basicPosture(int index);
    void turnDCOn(int index);
    void setPose(int index, QVector<int> servoAngles);
    void turnDCOff(int index);
    void disconnect(int index);

public slots:
    void runQueue();

signals:
    void workFinished();
    void connectTryFinished(int index, bool result);
    void basicPostureFinished(int index);
    void turnDCOnFinished(int index, bool result);
    void turnDCOffFinished(int index);
    void disconnected(int index);
};

#endif // SERIALPORTWORKER_H
