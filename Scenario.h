#ifndef SCENARIO_H
#define SCENARIO_H

#include <QVector>
#include <QString>

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
    QVector<Role> roles;
    QString musicFilePath;

public:
    Scenario();
    void addDanceFile(QString danceFilePath);
    void setMusic(QString t_musicFilePath);
    void addRole(int robotNum, int danceNum);

    QVector<QString> getDanceFilePaths();
    QVector<QString> getDanceFileNames();
    QString getMusicFilePath();
    QVector<Role> getRoles();

};

#endif // SCENARIO_H
