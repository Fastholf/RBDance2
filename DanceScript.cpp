#include "DanceScript.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>

bool DanceScript::skipLinesWithString(QTextStream *in,
                                      int linesCount,
                                      QString stringToFind)
{
    int count = 0;
    while (true) {
        if (in->atEnd()) {
            return false;
        }
        if (count == linesCount) {
            return true;
        }
        QString line = in->readLine();
        if (line == stringToFind) {
            ++count;
        }
    }
}

bool DanceScript::skipNLines(QTextStream *in, int linesCount)
{
    int count = 0;
    while (true) {
        QString temp = in->readLine();
        if (in->atEnd()) {
            return false;
        }
        ++count;
        if (count == linesCount) {
            return true;
        }
    }
}

QVector<int> DanceScript::parseStringToInts(QString buffer)
{
    QRegExp rx("[^0-9,]");
    buffer.replace(rx, "");
    QStringList items = buffer.split(",");

    QVector<int> result;
    for (int i = 0; i < items.count(); ++i) {
        bool ok;
        result.push_back(items[i].toInt(&ok));
    }

    return result;
}

bool DanceScript::loadPoseStepCounts(QTextStream *in)
{
    if (!skipLinesWithString(in, 5, "};")) {
        return false;
    }
    QString line = in->readLine();
    QString buffer = "";
    while (true) {
        if (in->atEnd()) {
            return false;
        }
        line = in->readLine();
        if (line == "};") {
            break;
        }
        buffer += line;
    }
    poseStepCounts = parseStringToInts(buffer);

    return true;
}

bool DanceScript::loadPoseDurations(QTextStream *in)
{
    QString line = in->readLine();
    QString buffer = "";
    while (true) {
        if (in->atEnd()) {
            return false;
        }
        line = in->readLine();
        if (line == "};") {
            break;
        }
        buffer += line;
    }
    poseDurations = parseStringToInts(buffer);

    return true;
}

bool DanceScript::loadPoseServoAngles(QTextStream *in)
{
    if (!skipNLines(in, 3)) {
        return false;
    }
    while (true) {
        if (in->atEnd()) {
            return false;
        }
        QString line = in->readLine();
        if (line == "};") {
            break;
        }
        QRegExp rx("[}].*");
        line = line.replace(rx, "");
        QVector<int> scene = parseStringToInts(line);
        poseServoAngles.push_back(scene);
    }

    return true;
}

void DanceScript::precountFrames()
{
    int poseCount = poseStepCounts.count();
    int currentTime = 0;
    frames.push_back(Frame(poseServoAngles[0], currentTime));

    for (int i = 1; i < poseCount; ++i)
    {
        int sleepBetweenSteps = poseDurations[i] / poseStepCounts[i];
        int servoCount = poseServoAngles[i].count();
        for (int j = 0; j < poseStepCounts[i]; ++j)
        {
            frames.push_back(Frame());
            int k = frames.count() - 1;
            frames[k].fireTime = currentTime + sleepBetweenSteps * (j + 1);
            for (int s = 0; s < servoCount; ++s)
            {
                frames[k].servoAngles.push_back(
                        interpolatedAngle(poseServoAngles[i - 1][s],
                                          poseServoAngles[i][s],
                                          poseStepCounts[i],
                                          j + 1)
                );
            }

        }
    }
}

int DanceScript::interpolatedAngle(int startAngle,
                                   int endAngle,
                                   int stepCount,
                                   int stepNum)
{
    int distance = endAngle - startAngle;
    double step = (double)distance / (double)stepCount;
    return startAngle + step * (double)stepNum;
}

DanceScript::DanceScript()
{
    clear();
}

void DanceScript::clear()
{
    filePath = "";
    poseStepCounts.clear();
    poseDurations.clear();
    poseServoAngles.clear();
    frames.clear();
}

bool DanceScript::loadFromMotionBuilderFile(QString t_filePath)
{
    filePath = t_filePath;
    QFile scripFile(filePath);
    if (!scripFile.open(QIODevice::ReadOnly)) {
        qCritical() << "File "
                    << filePath
                    << " with dance script was not found.";
        return false;
    }
    QTextStream in(&scripFile);

    if (!loadPoseStepCounts(&in)) {
        return false;
    }

    if (!loadPoseDurations(&in)) {
        return false;
    }

    if (!loadPoseServoAngles(&in)) {
        return false;
    }

    precountFrames();

    return true;
}

void DanceScript::reset()
{
    curIndex = 0;
}

int DanceScript::getCurrentFireTime()
{
    return frames[curIndex].fireTime;
}

Frame DanceScript::getCurrentFrame()
{
    return frames[curIndex];
}

bool DanceScript::isFinished()
{
    return curIndex == frames.count();
}

void DanceScript::goToNextFrame()
{
    ++curIndex;
}
