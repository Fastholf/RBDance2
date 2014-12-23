#ifndef SCENARIO_H
#define SCENARIO_H

#include <QVector>
#include <QString>
#include "DanceScript.h"
#include "FileLoadError.h"

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
    bool danceScriptsLoaded;
    QString errorMessage;

public:
    Scenario(int t_robotCount);
    void addDanceFile(QString danceFilePath);
    void setMusic(QString t_musicFilePath);
    void setMusicPlaying(bool t_musicPlaying);
    void setRole(int robotNum, int danceNum);
    FileLoadError loadDanceScripts(QString *t_errorMessage);
    bool isMusicPlaying();
    bool isReady(QString *t_errorMessage);

    QVector<QString> getDanceFilePaths();
    QVector<QString> getDanceFileNames();
    QString getMusicFilePath();
    QVector<Role> getRoles();
    QVector<DanceScript> getDanceScripts();

    QVector<int> involvedRobotNums();
    bool isMusicReady();

    void reset();
};

#endif // SCENARIO_H
