#pragma once
#include <QObject>
#include <memory>

namespace NS_Timer {

class SymbolsSettings;
class Timer;
class TimerData;
class KeyEventFilter;
class TrayIconWidget;
class DataBaseController;
class LogContext;

// This class is mediator between different classes
// Provide access for QML layer
// Transfers messages between Timer and database
class Controller : public QObject
{
	Q_OBJECT
public:
	explicit Controller(QObject* parent = nullptr);
	virtual ~Controller();

	void						Init();

	LogContext&			LogContext()														const	;
	SymbolsSettings&		SymbolsSettings()												const	;
	KeyEventFilter&		EventFilter()														const	;
	DataBaseController&	DatabaseController()												const	;
	Timer&					Timer()																const	;
	TrayIconWidget&		SysTrayWidget()													const	;

	// Returns a list of key symbols which enabled for tracking
	void						GetTrackingSymbolsList(QList<unsigned long>& list)	const;
	 // Returns path where aloceted database and log files
	void						GetDirectoryPath(QString& path)								const noexcept;
	bool						IsTimerWorking()													const noexcept;

	void						SendSetWindowsHook()											const;
	void						SendUnhookWindowsHook()									const;

	// Sends set key symbols for tracking
	void						SendSetSymbolListIntoFilter(const QList<unsigned long> &list)	const;
	void						SendControlKeyDetected();
	void						SendRecordData(const TimerData& data);
	void						SendSetTrayMenuActionEnable(const QString& actionName, bool isEnable);
	void						SendSetTrayIcon(const QString& icon_name);

public slots:
	// Sends events to Timer
	void						SendStartTimer();
	void						SendPauseTimer();
	void						SsendStopTimer();

private:
	std::shared_ptr<NS_Timer::LogContext>				logContext_;
	std::shared_ptr<NS_Timer::SymbolsSettings>		symbolsSettings_;
	std::shared_ptr<NS_Timer::KeyEventFilter>			eventFilter_;
	std::shared_ptr<NS_Timer::DataBaseController>	dbController_;
	std::shared_ptr<NS_Timer::Timer>						timer_;
	std::shared_ptr<NS_Timer::TrayIconWidget>		trayIcon_;
	QString															appPath_; // path for database and log files
};

}//namespace NS_Timer
