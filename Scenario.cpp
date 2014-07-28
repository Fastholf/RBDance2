#include "Scenario.h"
#include <QFile>
#include <QFileInfo>

Scenario::Scenario()
{

}

void Scenario::addDanceFile(QString danceFilePath)
{
    danceFilePaths.push_back(danceFilePath);
}

void Scenario::setMusic(QString t_musicFilePath)
{
    musicFilePath = t_musicFilePath;
}

void Scenario::addRole(int robotNum, int danceNum)
{
    roles.push_back(Role(robotNum, danceNum));
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
