#ifndef DANCESCRIPT_H
#define DANCESCRIPT_H

#include <QVector>
#include <QString>
#include <QTextStream>

class Frame
{
public:
    QVector<int> servoAngles;
    int fireTime;
    Frame() {}
    Frame(QVector<int> t_servoAngles, int t_fireTime)
    {
        servoAngles = t_servoAngles;
        fireTime = t_fireTime;
    }
};

class DanceScript
{
private:
    QString filePath;
    QVector<int> poseStepCounts;
    QVector<int> poseDurations;
    QVector< QVector<int> > poseServoAngles;
    QVector<Frame> frames;

    bool skipLinesWithString(QTextStream *in,
                             int linesCount,
                             QString stringToFind);
    bool skipNLines(QTextStream *in, int linesCount);
    QVector<int> parseStringToInts(QString buffer);
    bool loadPoseStepCounts(QTextStream *in);
    bool loadPoseDurations(QTextStream *in);
    bool loadPoseServoAngles(QTextStream *in);
    void precountFrames();
    int interpolatedAngle(int startAngle,
                          int endAngle,
                          int stepCount,
                          int stepNum);

public:
    DanceScript();
    void clear();
    bool loadFromMotionBuilderFile(QString t_filePath);
};

#endif // DANCESCRIPT_H
