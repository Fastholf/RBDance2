#include "Scenario.h"
#include <QFile>
#include <QFileInfo>
#include <limits>
#include <algorithm>    // std::min

Scenario::Scenario(int t_robotCount)
{
    robotCount = t_robotCount;
    for (int i = 0; i < robotCount; ++i) {
        roles.push_back(Role(i, -1));
    }
    for (int i = 0; i < robotCount; ++i) {
        danceScripts.push_back(DanceScript());
    }
}

void Scenario::addDanceFile(QString danceFilePath)
{
    danceFilePaths.push_back(danceFilePath);
}

void Scenario::setMusic(QString t_musicFilePath)
{
    musicFilePath = t_musicFilePath;
}

void Scenario::setRole(int robotNum, int danceNum)
{
    for (int i = 0; i < roles.count(); ++i) {
        if (roles[i].robotNum == robotNum) {
            roles[i].danceNum = danceNum;
        }
    }
}

bool Scenario::loadDanceScripts()
{
    for (int i = 0; i < danceFilePaths.count(); ++i) {
        danceScripts[i].clear();
        bool used = false;
        for (int j = 0; j < roles.count(); ++j) {
            if (roles[j].danceNum == i)  {
                used = true;
                break;
            }
        }
        if (!used) {
            continue;
        }
        if (!danceScripts[i].loadFromMotionBuilderFile(danceFilePaths[i])) {
            return false;
        }
    }
    return true;
}

QVector<QString> Scenario::getDanceFilePaths()
{
    return danceFilePaths;
}

QVector<QString> Scenario::getDanceFileNames()
{
    QVector<QString> danceFileNames;
    for (int i = 0; i < danceFilePaths.count(); ++i) {
        QFile f(danceFilePaths[i]);
        QFileInfo fileInfo(f.fileName());
        QString filename(fileInfo.fileName());
        danceFileNames.push_back(filename);
    }
    return danceFileNames;
}

QString Scenario::getMusicFilePath()
{
    return musicFilePath;
}

QVector<Role> Scenario::getRoles()
{
    return roles;
}

QVector<DanceScript> Scenario::getDanceScripts()
{
    return danceScripts;
}

int Scenario::minFireTime()
{
    int result = std::numeric_limits<int>::max();
    for (int i = 0; i < roles.count(); ++i) {
        if (roles[i].danceNum != -1) {
            int time = danceScripts[roles[i].danceNum].getCurrentFireTime();
            result = std::min(result, time);
        }
    }

    return result;
}
