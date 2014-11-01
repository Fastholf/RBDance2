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
    bool musicPlaying;
    int robotCount;

public:
    Scenario(int t_robotCount);
    void addDanceFile(QString danceFilePath);
    void setMusic(QString t_musicFilePath);
    void setMusicPlaying(bool t_musicPlaying);
    void setRole(int robotNum, int danceNum);
    bool loadDanceScripts(QString *errorMessage);
    bool isMusicPlaying();

    QVector<QString> getDanceFilePaths();
    QVector<QString> getDanceFileNames();
    QString getMusicFilePath();
    QVector<Role> getRoles();
    QVector<DanceScript> getDanceScripts();

    QVector<int> involvedRobotNums();
    bool isMusicReady();
};

#endif // SCENARIO_H
