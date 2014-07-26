#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString.h>
#include <QVector>
#include <QObject>
#include "Robot.h"

enum FileLoadError {
    FileLoadErrorNo = 0,
    FileLoadErrorNotFound,
    FileLoadErrorWrongFormat,
};

class Controller : public QObject
{
    Q_OBJECT

private:
    QString robotsFileName;
    QVector<Robot> robots;

public:
    explicit Controller();
    FileLoadError LoadRobotsFromFile();
signals:
    void robotLoaded(int index, QString name, int port);
};

#endif // CONTROLLER_H
