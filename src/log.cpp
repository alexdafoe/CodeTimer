#include "log.h"
#include <QTextStream>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

namespace NS_Timer
{

QDateTime LogContext::logTime	= QDateTime::currentDateTime();
QFile LogContext::logFile;

// Message handler
void messageToFile(QtMsgType							type,
						   const QMessageLogContext&	context,
						   const QString&						msg);

LogContext::LogContext(const QString& _workDirictory)
: QObject(nullptr)
, workDirictory_(_workDirictory)
, logFileName_("protocol.log")
{
	// Install function as a message handler of all debug message
	qInstallMessageHandler(messageToFile); // comment row for print log in standart tab IDE
}

void LogContext::Init() {
	if(logFile.isOpen())
		logFile.close();

	logFile.setFileName(workDirictory_ + logFileName_);
	if(!logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
		qWarning() << "can't open log file";
		return;
	}
	qDebug() << "Start session =================================================";
}

void LogContext::Done()
{
	qDebug() << "End session =================================================";
	logFile.flush();
	logFile.close();
}

void LogContext::removeLogFile() {
	if(logFile.exists()){
		logFile.close();
		logFile.remove();
	}
	Init();
}

void LogContext::openLogPath() {
	QFileInfo logFilePath(logFile.fileName());
	QDesktopServices::openUrl(QUrl(logFilePath.absoluteFilePath(), QUrl::TolerantMode));
}

void messageToFile(QtMsgType _type, const QMessageLogContext& _context, const QString& _msg) {
	if(!LogContext::logFile.isOpen()) {
		return;
	}

	QTextStream out(&LogContext::logFile);
	LogContext::logTime = QDateTime::currentDateTime();
	switch (_type) {
	case QtDebugMsg:
		out << LogContext::logTime.toString("yy.MM.dd hh:mm:ss") << " " << _msg << ", " << _context.file << endl;
		break;
	case QtWarningMsg:
		out << LogContext::logTime.toString("yy.MM.dd hh:mm:ss") << " Warning: " << _msg << ", " << _context.file << endl;
		break;
	case QtCriticalMsg:
		out << LogContext::logTime.toString("yy.MM.dd hh:mm:ss") << " Critical: " << _msg << ", " << _context.file << endl;
		break;
	case QtFatalMsg:
		out << LogContext::logTime.toString("yy.MM.dd hh:mm:ss") << " Fatal: " << _msg << ", " << _context.file << endl;
		std::abort();
	}
}

}//namespace NS_Timer
