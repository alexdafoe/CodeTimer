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
	Timer(const Timer&)																		= delete;
	Timer(Timer&&)																			= delete;
	virtual ~Timer();

	Timer&			operator=(const Timer&)											= delete;
	Timer&			operator=(Timer&&)													= delete;

	// String that shows start counting of time. Propagated to QML
	Q_PROPERTY(QString			TimeStartStr
						READ				TimeStart
						NOTIFY			timeStartChanged)
	QString			TimeStart()																const;

	// The main timer. Propagated to QML
	Q_PROPERTY(QString			TimerStr
						READ				TimerStr
						CONSTANT)
	QString			TimerStr();

	// String that shows time of writing code counting by key symbols. Propagated to QML
	Q_PROPERTY(QString			TimeLeftStr
						READ				TimeWritingCodeStr
						CONSTANT)
	QString			TimeWritingCodeStr();

	// State of main timer. Propagated to QML
	Q_PROPERTY(bool					IsTimerWorking
						READ				IsTimerWorking
						WRITE			TimerWorking
						NOTIFY			timerWorkingStateChanged)
	bool				IsTimerWorking()														const noexcept { return isTimerWorking_; }
	void				TimerWorking(bool trigger);

	// State of main timer for correct shows labels in QML layers. Not change on pause. Propagated to QML
	Q_PROPERTY(bool					IsSessionOngoing
						READ				IsSessionOngoing
						WRITE			SessionOngoing
						NOTIFY			timeStartStatusChanged)
	bool				IsSessionOngoing()													const noexcept { return isSessionOngoing_; }
	void				SessionOngoing(bool state);

	// Maximum seconds between key symbols detection events
	Q_PROPERTY(unsigned int		MaxPauseDurationSec
						READ				MaxPauseDurationSec
						WRITE			MaxPauseDurationSec
						NOTIFY			maxPauseDurationSecChanged)
	unsigned int	MaxPauseDurationSec()												const noexcept { return maxPauseDurationSec_; }
	void				MaxPauseDurationSec(unsigned int durationSec);

	// Event from KeyEventFilter that special key detected. Count time of writing code
	void				SetTimePoint();
	// Prepare "working time" for record in TimerData
	void				CountWorkingTime();

public slots:
	// Propagated to QML
	void				start();
	void				pause();
	void				stop();

signals:
	void				timerTrigger();
	void				maxPauseDurationSecChanged(unsigned int);
	void				timerWorkingStateChanged(bool);
	void				timeWritingCodeChanged();
	void				timeStartStatusChanged();
	void				timeStartChanged();

protected:
	void				FillTimerData();
	void				RecordTimerData();

private:
	using TimePoint			= std::chrono::time_point<std::chrono::system_clock>;
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
