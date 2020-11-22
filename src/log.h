#pragma once
#include <QObject>
#include <QtMsgHandler>
#include <QString>
#include <QFile>
#include <QDateTime>

namespace NS_Timer
{

class Controller;

// Message handler
void messageToFile(QtMsgType							type,
						   const QMessageLogContext&	context,
						   const QString&						msg);

// This class is a file manager for log
class LogContext : public QObject
{
	Q_OBJECT
public:
	LogContext(QObject* parent, Controller*);
	virtual ~LogContext();

	void						Init(const QString& logPath);

	// Full log file name. Propagated to QML
	Q_PROPERTY(QString	logFileName_
						READ		LogFileName
						CONSTANT)
	QString					LogFileName()				const;

	// Public static variable for message handler
	static QDateTime		logTime;
	static QFile				logFile;

public slots:
	void						RemoveLogFile();
	void						OpenLogPath();

private:
	Controller*				controller_;
	QString					logFileName_;
};

}//namespace NS_Timer
