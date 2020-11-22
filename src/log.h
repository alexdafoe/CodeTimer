#pragma once
#include <QObject>
#include <QtMsgHandler>
#include <QString>
#include <QFile>
#include <QDateTime>

class Controller;

// Message handler
void messageToFile(QtMsgType							type,
						   const QMessageLogContext&	context,
						   const QString&						msg);

// This class is a file manager for log
class Log : public QObject
{
	Q_OBJECT
public:
	Log(QObject* parent, Controller* controller);
	virtual ~Log();

	void						init(const QString &path);

	// Full log file name. Propagated to QML
	Q_PROPERTY(QString	logFileName
						READ		getLogFileName
						CONSTANT)
	QString					getLogFileName()				const;

	// Public static variable for message handler
	static QDateTime		_logTime;
	static QFile				currentLogFile;

public slots:
	void						removeCurrentLogFile();
	void						openLogFileFolder();

private:
	Controller*				_controller;
	QString					_logFileName;
};
