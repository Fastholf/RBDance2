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

DanceScript::DanceScript()
{
    clear();
}

void DanceScript::clear()
{
    filePath = "";
    frames.clear();
    times.clear();
    positions.clear();
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
    if (!skipLinesWithString(&in, 5, "};")) {
        return false;
    }
    QString line = in.readLine();
    QString buffer = "";
    while (true) {
        if (in.atEnd()) {
            return false;
        }
        line = in.readLine();
        if (line == "};") {
            break;
        }
        buffer += line;
    }
    frames = parseStringToInts(buffer);

    line = in.readLine();
    buffer = "";
    while (true) {
        if (in.atEnd()) {
            return false;
        }
        line = in.readLine();
        if (line == "};") {
            break;
        }
        buffer += line;
    }
    times = parseStringToInts(buffer);

    if (!skipNLines(&in, 3)) {
        return false;
    }
    while (true) {
        if (in.atEnd()) {
            return false;
        }
        line = in.readLine();
        if (line == "};") {
            break;
        }
        QRegExp rx("[}].*");
        line = line.replace(rx, "");
        QVector<int> scene = parseStringToInts(line);
        positions.push_back(scene);
    }
    qDebug() << positions;

    return true;
}
