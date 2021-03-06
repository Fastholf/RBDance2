#include "DanceScript.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>
#include <algorithm>

bool DanceScript::skipLinesWithString(QTextStream *in,
                                      int linesCount,
                                      QString stringToFind)
{
//    qDebug() << "Method name";

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
//    qDebug() << "Method name";

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
//    qDebug() << "Method name";

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
//    qDebug() << "Method name";

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
//    qDebug() << "Method name";

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
//    qDebug() << "Method name";

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
//    qDebug() << "Method name";

    int poseCount = poseStepCounts.count();
    int currentTime = 0;
    frames.push_back(Frame(poseServoAngles[0], currentTime));

    for (int i = 0; i < poseCount; ++i)
    {
        int sleepBetweenSteps = std::max(poseDurations[i] / poseStepCounts[i], 25);
        int servoCount = poseServoAngles[i + 1].count();
        for (int j = 1; j <= poseStepCounts[i]; ++j)
        {
            frames.push_back(Frame());
            int k = frames.count() - 1;
            frames[k].fireTime = currentTime + sleepBetweenSteps * j;
            for (int s = 0; s < servoCount; ++s)
            {
                frames[k].servoAngles.push_back(
                        interpolatedAngle(poseServoAngles[i][s],
                                          poseServoAngles[i + 1][s],
                                          poseStepCounts[i],
                                          j)
                );
            }

        }
        currentTime += poseDurations[i];
    }
}

int DanceScript::interpolatedAngle(int startAngle,
                                   int endAngle,
                                   int stepCount,
                                   int stepNum)
{
//    qDebug() << "Method name";

    int distance = endAngle - startAngle;
    double step = (double)distance / (double)stepCount;
    return startAngle + step * (double)stepNum;
}

DanceScript::DanceScript()
{
//    qDebug() << "Method name";

    clear();
}

void DanceScript::clear()
{
//    qDebug() << "Method name";

    filePath = "";
    poseStepCounts.clear();
    poseDurations.clear();
    poseServoAngles.clear();
    frames.clear();
}

FileLoadError DanceScript::loadFromMotionBuilderFile(QString t_filePath)
{
//    qDebug() << "Method name";

    filePath = t_filePath;
    QFile scripFile(filePath);
    if (!scripFile.open(QIODevice::ReadOnly)) {
        qCritical() << "File "
                    << filePath
                    << " with dance script was not found.";
        return FileLoadErrorNotFound;
    }
    QTextStream in(&scripFile);

    if (!loadPoseStepCounts(&in)) {
        return FileLoadErrorWrongFormat;
    }

    if (!loadPoseDurations(&in)) {
        return FileLoadErrorWrongFormat;
    }

    if (!loadPoseServoAngles(&in)) {
        return FileLoadErrorWrongFormat;
    }

    precountFrames();

    return FileLoadErrorNo;
}

int DanceScript::getFramesCount()
{
    return frames.count();
}

Frame DanceScript::getFrameAtIndex(int index)
{
    return frames[index];
}

int DanceScript::getFireTimeAtIndex(int index)
{
    return frames[index].fireTime;
}
