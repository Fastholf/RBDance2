#include "Scenario.h"
#include <QFile>
#include <QFileInfo>
#include <limits>
#include <algorithm>    // std::min
#include <QDebug>

Scenario::Scenario(int t_robotCount)
{
//    qDebug() << "Method name";

    robotCount = t_robotCount;

    reset();
}

void Scenario::reset()
{
    roles.clear();
    for (int i = 0; i < robotCount; ++i) {
        roles.push_back(Role(i, -1));
    }

    danceFilePaths.clear();
    danceScripts.clear();
    for (int i = 0; i < robotCount; ++i) {
        danceScripts.push_back(DanceScript());
    }

    musicFilePath = "";
    musicPlaying = false;

    errorMessage = "";
    danceScriptsLoaded = false;
}

void Scenario::addDanceFile(QString danceFilePath)
{
//    qDebug() << "Method name";

    danceFilePaths.push_back(danceFilePath);
}

void Scenario::setMusic(QString t_musicFilePath)
{
//    qDebug() << "Method name";

    musicFilePath = t_musicFilePath;
}

void Scenario::setMusicPlaying(bool t_musicPlaying)
{
//    qDebug() << "Method name";

    musicPlaying = t_musicPlaying;
}

void Scenario::setRole(int robotNum, int danceNum)
{
//    qDebug() << "Method name";

    for (int i = 0; i < roles.count(); ++i) {
        if (roles[i].robotNum == robotNum) {
            roles[i].danceNum = danceNum;
        }
    }
}

FileLoadError Scenario::loadDanceScripts(QString *t_errorMessage)
{
//    qDebug() << "Method name";

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
        FileLoadError result = danceScripts[i].loadFromMotionBuilderFile(danceFilePaths[i]);
        if (result != FileLoadErrorNo) {
            if (result == FileLoadErrorNotFound) {
                errorMessage = "Can not find file " + danceFilePaths[i];
            }
            else if (result == FileLoadErrorWrongFormat) {
                errorMessage = "File " + danceFilePaths[i] + " has wrong format.";
            }
            *t_errorMessage = errorMessage;
            danceScriptsLoaded = false;
            return result;
        }
    }
    danceScriptsLoaded = true;
    return FileLoadErrorNo;
}

bool Scenario::isMusicPlaying()
{
//    qDebug() << "Method name";

    return musicPlaying;
}

bool Scenario::isReady(QString *t_errorMessage)
{
    if (!danceScriptsLoaded) {
        *t_errorMessage = errorMessage;
        return false;
    }

    if (!isMusicReady()) {
        errorMessage = "Music file " + musicFilePath + " was not found.";
        *t_errorMessage = errorMessage;
        return false;
    }

    return true;
}

QVector<QString> Scenario::getDanceFilePaths()
{
//    qDebug() << "Method name";

    return danceFilePaths;
}

QVector<QString> Scenario::getDanceFileNames()
{
//    qDebug() << "Method name";

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
//    qDebug() << "Method name";

    return musicFilePath;
}

QVector<Role> Scenario::getRoles()
{
//    qDebug() << "Method name";

    return roles;
}

QVector<DanceScript> Scenario::getDanceScripts()
{
//    qDebug() << "Method name";

    return danceScripts;
}

QVector<int> Scenario::involvedRobotNums()
{
//    qDebug() << "Method name";

    QVector<int> result;
    for (int i = 0; i < roles.count(); ++i) {
        if (roles[i].danceNum != -1) {
            result.append(roles[i].robotNum);
        }
    }
    return result;
}

bool Scenario::isMusicReady()
{
//    qDebug() << "Method name";

    if (!musicPlaying) {
        return true;
    }

    QFile musicFile(musicFilePath);
    return musicFile.exists();
}
