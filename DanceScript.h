#ifndef DANCESCRIPT_H
#define DANCESCRIPT_H

#include <QVector>
#include <QString>
#include <QTextStream>

class DanceScript
{
private:
    QString filePath;
    QVector<int> frames;
    QVector<int> times;
    QVector< QVector<int> > positions;

    bool skipLinesWithString(QTextStream *in,
                             int linesCount,
                             QString stringToFind);
    bool skipNLines(QTextStream *in, int linesCount);
    QVector<int> parseStringToInts(QString buffer);

public:
    DanceScript();
    void clear();
    bool loadFromMotionBuilderFile(QString t_filePath);
};

#endif // DANCESCRIPT_H
