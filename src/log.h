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
	LogContext(const LogContext&&)								= delete;
	virtual ~LogContext();

	LogContext&			operator=(const LogContext&)		= delete;
	LogContext&			operator=(const LogContext&&)		= delete;

	void						Init();

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
