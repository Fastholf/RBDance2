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
     QString txt;
    if (msg != "Method name") {
        QByteArray localMsg = msg.toLocal8Bit();
        txt = QString(" %1 (%2:%3, %4)").arg(QString(localMsg.constData()),
                                               QString(context.file),
                                               QString::number(context.line),
                                               QString(context.function));
        switch (type) {
        case QtDebugMsg:
            txt = "[DBG]:" + txt;
            break;
        case QtWarningMsg:
            txt = "[WRN]:" + txt;
            break;
        case QtCriticalMsg:
            txt = "[CRITICAL]:" + txt;
            break;
        case QtFatalMsg:
            txt = "[FTATAL]:" + txt;
    //        abort();
        }
	}
    else {
        txt = QString(context.function);
    }

    QDateTime dateTime = QDateTime::currentDateTime();
    txt = QString("[%1]: %2").arg(dateTime.toString("yyyy-MM-dd hh:mm:ss"), txt);

#ifndef WIN32
    fprintf(stderr, txt.toStdString().c_str());
#else
    QString message = txt + "\n";
    OutputDebugString(reinterpret_cast<const wchar_t *>(message.utf16()));
#endif
    QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
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
