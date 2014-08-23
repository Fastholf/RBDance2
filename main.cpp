#include "MainWindow.h"
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QtGlobal>

#ifdef WIN32
    #include <Windows.h>
#endif

void myMessageOutput(QtMsgType type,
                     const QMessageLogContext &context,
                     const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString txt = QString(" %1 (%2:%3, %4)\n").arg(QString(localMsg.constData()),
                                                   QString(context.file),
                                                   QString::number(context.line),
                                                   QString(context.function));
    switch (type) {
    case QtDebugMsg:
        txt = "Debug:" + txt;
        break;
    case QtWarningMsg:
        txt = "Warining:" + txt;
        break;
    case QtCriticalMsg:
        txt = "Critical:" + txt;
        break;
    case QtFatalMsg:
        txt = "Fatal:" + txt;
//        abort();
    }
#ifndef WIN32
    fprintf(stderr, txt.toStdString().c_str());
#else
    QString message = msg + "\n";
    OutputDebugString(reinterpret_cast<const wchar_t *>(message.utf16()));
#endif
    QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(myMessageOutput);
    qDebug() << "\n\n\n===Application started===";

    MainWindow w;
    w.show();

    return a.exec();
}
