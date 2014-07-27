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
    QString scenarioListFileName;
    QVector<Robot> robots;
    QVector<QString> scenarioPaths;

public:
    explicit Controller();
    FileLoadError loadRobotsFromFile();
    FileLoadError loadScenarioListFromFile();
signals:
    void robotLoaded(int index, QString name, int port);
    void scenarioListLoaded(QVector<QString> scenarioPaths);
};

#endif // CONTROLLER_H
