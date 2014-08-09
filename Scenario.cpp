#include "Scenario.h"
#include <QFile>
#include <QFileInfo>

Scenario::Scenario(int t_robotCount)
{
    robotCount = t_robotCount;
    for (int i = 0; i < robotCount; ++i) {
        roles.push_back(Role(i, -1));
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
