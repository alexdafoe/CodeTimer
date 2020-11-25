#pragma once
#include <QObject>
#include <QString>
#include <memory>

namespace NS_Timer {

class Timer;
class TimerData;
class KeyEventFilter;
class TrayIconWidget;
class SymbolsSettings;
class LogContext;
class DatabaseWrap;
class DatabaseModel;

// This class is mediator between different classes
// Provide access for QML layer
// Transfers messages between Timer and database
class Controller : public QObject
{
	Q_OBJECT
public:
	Controller();
	Controller(const Controller&)										= delete;
	Controller(Controller&&)												= delete;
	virtual ~Controller();

	Controller&				operator=(const Controller&)			= delete;
	Controller&				operator=(Controller&&)					= delete;

	void						Init();

	LogContext&			Log()											const	;
	SymbolsSettings&		KeyControlSettings()						const	;
	KeyEventFilter&		EventFilter()								const	;
	DatabaseWrap&		DB()											const;
	DatabaseModel&		DBModel()									const;
	Timer&					TimerControl()								const	;
	TrayIconWidget&		SysTrayWidget()							const	;

	// Sends set key symbols for tracking
	void						SendControlKeyDetected();

public slots:
	// Sends events to Timer
	void						SendStartTimer();
	void						SendPauseTimer();
	void						SendStopTimer();
	//db
	void						UpdateModel();

private:
	QString											appPath_; // path for database and log files
	std::shared_ptr<LogContext>				logContext_;
	std::shared_ptr<DatabaseModel>		dbModel_;
	std::shared_ptr<DatabaseWrap>			dbWrap_;
	std::shared_ptr<KeyEventFilter>			eventFilter_;
	std::shared_ptr<SymbolsSettings>		symbolsSettings_;
	std::shared_ptr<Timer>						timer_;
	std::shared_ptr<TrayIconWidget>		trayIcon_;
};

}//namespace NS_Timer
