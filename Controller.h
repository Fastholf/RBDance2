#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString.h>
#include <QVector>
#include <QObject>
#include "Robot.h"
#include "Scenario.h"

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
    Scenario *scenario;

public:
    explicit Controller();
    FileLoadError loadRobotsFromFile();
    FileLoadError loadScenarioListFromFile();
    FileLoadError loadScenarioFromFile(int scenarioIndex);

signals:
    void robotLoaded(int index, QString name, int port);
    void scenarioListLoaded(QVector<QString> scenarioPaths);
    void scenarioLoaded(QVector<QString> danceFileNames, QVector<Role> roles);

};

#endif // CONTROLLER_H
