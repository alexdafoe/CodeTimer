#pragma once
#include <QObject>
#include <QtMsgHandler>
#include <QString>
#include <QFile>
#include <QDateTime>

namespace NS_Timer
{

// This class is a file manager for log
class LogContext : public QObject
{
	Q_OBJECT
public:
	explicit LogContext(const QString& workDirictory);
	LogContext(const LogContext&)									= delete;
	LogContext(LogContext&&)										= delete;
	virtual ~LogContext()												= default;

	LogContext&			operator=(const LogContext&)		= delete;
	LogContext&			operator=(LogContext&&)				= delete;

	void						Init();
	void						Done();

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
	QString					workDirictory_;
	QString					logFileName_;
};

}//namespace NS_Timer
