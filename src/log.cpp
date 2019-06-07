#include "include/log.h"
#include "include/controller.h"
#include <QTextStream>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
QDateTime Log::_logTime = QDateTime::currentDateTime();
QFile Log::currentLogFile;

Log::Log(QObject *parent, Controller *controller) :
   QObject(parent),
   _controller(controller),
   _logFileName("protocol.log")
{
    //qInstallMessageHandler(messageToFile);
}

Log::~Log() {
    qDebug() << "End session =================================================";
    currentLogFile.flush();
    currentLogFile.close();
}

void Log::initLogFile(const QString &path) {
    if(currentLogFile.isOpen())
        currentLogFile.close();

    currentLogFile.setFileName(path + _logFileName);
    if(!currentLogFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        qWarning() << "can't open log file";
        return;
    }
    qDebug() << "Start session =================================================";
}

QString Log::getLogFileName() const {
    return _logFileName;
}

void Log::removeCurrentLogFile() {
    if(currentLogFile.exists()){
        currentLogFile.close();
        currentLogFile.remove();
    }
    if(_controller != nullptr){
        QString path;
        _controller->getDirectoryPath(path);
        initLogFile(path);
    }
}

void Log::openLogFileFolder() {
    QFileInfo logFilePath(currentLogFile.fileName());
    QDesktopServices::openUrl(QUrl(logFilePath.absoluteFilePath(), QUrl::TolerantMode));
}

void messageToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if(!Log::currentLogFile.isOpen())
        return;
    QTextStream out(&Log::currentLogFile);
    Log::_logTime = QDateTime::currentDateTime();
    switch (type) {
    case QtDebugMsg:
        out << Log::_logTime.toString("yy.MM.dd hh:mm:ss") << " " << msg << ", " << context.file << endl;
        break;
    case QtWarningMsg:
        out << Log::_logTime.toString("yy.MM.dd hh:mm:ss") << " Warning: " << msg << ", " << context.file << endl;
        break;
    case QtCriticalMsg:
        out << Log::_logTime.toString("yy.MM.dd hh:mm:ss") << " Critical: " << msg << ", " << context.file << endl;
        break;
    case QtFatalMsg:
        out << Log::_logTime.toString("yy.MM.dd hh:mm:ss") << " Fatal: " << msg << ", " << context.file << endl;
        std::abort();
    //default:
        //break;
    }
}
