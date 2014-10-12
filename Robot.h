#ifndef ROBOT_H
#define ROBOT_H

#include <QString>
#include <QVector>
#include <QtSerialPort/QtSerialPort>
#include "RBController.h"
#include "SerialPortWorker.h"

class Robot : public QObject
{
    Q_OBJECT

private:
    int _index;
    int portNum;
    QString name;
    SerialPortWorker *_portWorker;

    bool _connected;
    bool _DCModeOn;
    RBController *rbController;


public:
    Robot();
    Robot(int index,
          int t_portNum,
          QString t_name,
          SerialPortWorker *portWorker);
    QString getName();
    void connectToRB();
    void basicPosture();
    void turnDCOn();
    void setPose(QVector<int> servoAngles);
    void turnDCOff();
    void disconnect();

    bool isConnected();
    bool isDCModeOn();

signals:
    void connectTryFinished(int index, bool result);
    void turnDCOnFinished(int index, bool result);
    void turnDCOffFinished(int index);
    void disconnected(int index);

public slots:
    void onConnectTryFinished(int index, bool result);
    void onTurnDCOnFinished(int index, bool result);
    void onTurnDCOffFinished(int index);
    void onDisconnected(int index);
};

#endif // ROBOT_H
