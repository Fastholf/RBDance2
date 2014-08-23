#ifndef SCENARIO_H
#define SCENARIO_H

#include <QVector>
#include <QString>
#include "DanceScript.h"

class Role
{
public:
    int robotNum, danceNum;
    Role() {}
    Role(int t_robotNum, int t_danceNum)
    {
        robotNum = t_robotNum;
        danceNum = t_danceNum;
    }
};

class Scenario
{
private:
    QVector<QString> danceFilePaths;
    QVector<DanceScript> danceScripts;
    QVector<Role> roles;
    QString musicFilePath;
    int robotCount;

public:
    Scenario(int t_robotCount);
    void addDanceFile(QString danceFilePath);
    void setMusic(QString t_musicFilePath);
    void setRole(int robotNum, int danceNum);
    bool loadDanceScripts(QString *errorMessage);

    QVector<QString> getDanceFilePaths();
    QVector<QString> getDanceFileNames();
    QString getMusicFilePath();
    QVector<Role> getRoles();
    QVector<DanceScript> getDanceScripts();

    int minFireTime();
    QVector<int> involvedRobotNums();
};

#endif // SCENARIO_H
