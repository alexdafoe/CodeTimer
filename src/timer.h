#pragma once
#include "controller.h"
#include "timerdata.h"

#include <QObject>
#include <QTimer>
#include <chrono>
#include <ctime>
#include <functional>

namespace NS_Timer
{

// This class is a main Timer
// Count time between start/pause/stop and key symbols detected event
class Timer : public QObject
{
	Q_OBJECT
public:
	explicit Timer(std::reference_wrapper<Controller>);
	Timer(const Timer&)															= delete;
	Timer(Timer&&)																= delete;
	virtual ~Timer();

	Timer&			operator=(const Timer&)								= delete;
	Timer&			operator=(Timer&&)										= delete;

	// String that shows start counting of time. Propagated to QML
	Q_PROPERTY(QString			timeStartStr
						READ				TimeStart
						NOTIFY			TimeStartChanged)
	QString			TimeStart()															const;

	// The main timer. Propagated to QML
	Q_PROPERTY(QString			timerStr
						READ				TimeStr
						CONSTANT)
	QString			TimeStr();

	// String that shows time of writing code counting by key symbols. Propagated to QML
	Q_PROPERTY(QString			timeWritingCodeStr
						READ				TimeWritingCodeStr
						CONSTANT)
	QString			TimeWritingCodeStr();

	// State of main timer. Propagated to QML
	Q_PROPERTY(bool					isTimerWorking
						READ				IsTimerWorking
						WRITE			TimerWorking
						NOTIFY			TimerWorkingStateChanged)
	bool				IsTimerWorking()														const noexcept;
	void				TimerWorking(bool trigger);

	// State of main timer for correct shows labels in QML layers. Not change on pause. Propagated to QML
	Q_PROPERTY(bool					isSessionOngoing_
						READ				IsSessionOngoing
						WRITE			SessionOngoing
						NOTIFY			TimeStartStatusChanged)
	bool				IsSessionOngoing()													const noexcept;
	void				SessionOngoing(bool state);

	// Maximum seconds between key symbols detection events
	Q_PROPERTY(unsigned int		maxPauseDurationSec_
						READ				MaxPauseDurationSec
						WRITE			MaxPauseDurationSec
						NOTIFY			MaxPauseDurationSecChanged)
	unsigned int	MaxPauseDurationSec()											const noexcept;
	void				MaxPauseDurationSec(unsigned int durationSec);

	// Event from KeyEventFilter that special key detected. Count time of writing code
	void				SetTimePoint();
	// Prepare "working time" for record in TimerData
	void				CountWorkingTime();

public slots:
	// Propagated to QML
	void				Start();
	void				Pause();
	void				Stop();

signals:
	void				TimerTrigger();
	void				MaxPauseDurationSecChanged(unsigned int);
	void				TimerWorkingStateChanged(bool);
	void				TimeWritingCodeChanged();
	void				TimeStartStatusChanged();
	void				TimeStartChanged();

protected:
	void				FillTimerData();
	void				RecordTimerData();

private:
	using TimePoint	= std::chrono::time_point<std::chrono::system_clock>;
	using TimeDuration		= std::chrono::duration<double>;

	std::reference_wrapper<Controller>		controller_;
	TimePoint										lastActive_;
	TimePoint										startWork_;
	TimeDuration									durationWritingCode_;
	TimeDuration									durationTimeWorking_;
	TimerData										timerData_;
	unsigned int									maxPauseDurationSec_	= 300; // 5 min
	bool												buttonPausePressed_	= false;
	bool												isTimerWorking_			= false;
	bool												isSessionOngoing_		= false;
	QTimer											trigger_; // call signal timerTrigger every 1 sec for refresh clock in QML layer
	QString											timerStr_					= "0";
	QString											timeLeftStr_;
};

}//namespace NS_Timer
