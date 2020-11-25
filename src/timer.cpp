#include "timer.h"
#include "keyeventfilter.h"
#include "trayiconwidget.h"
#include "timesectostring.h"
#include "database.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

namespace NS_Timer
{

using namespace std::chrono;

Timer::Timer(std::reference_wrapper<Controller> _controller)
: QObject(nullptr)
, controller_(_controller)
{
	// refresh clock every 1 sec (main.qml onTimerTrigger)
	trigger_.start(1000);
	QObject::connect(&trigger_, SIGNAL(timeout()), this, SIGNAL(TimerTrigger()));
	durationWritingCode_ = duration<double>(0);
}

Timer::~Timer() {
	if(isSessionOngoing_) Stop(); //even if on pause
}

QString Timer::TimeStart() const {
	return timerData_.TimeStart();
}

QString Timer::TimeStr() {
	auto now = time_point_cast<seconds>(system_clock::now());
	auto realTime = now - lastActive_;
	double time = durationWritingCode_.count();
	if(isTimerWorking_){
		// SEC_DIVIDER is a compiler dependent variable
		// calculation std::chrono::duration to seconds
		// established in .pro file
		time += realTime.count()/SEC_DIVIDER;
	}
	TimeSecToString()((int)time, timerStr_);
	return timerStr_;
}

QString Timer::TimeWritingCodeStr() {
	TimeSecToString()(int(durationWritingCode_.count()), timeLeftStr_);
	return timeLeftStr_;
}

bool Timer::IsTimerWorking() const noexcept{
	return isTimerWorking_;
}

void Timer::TimerWorking(bool _trigger) {
	if(isTimerWorking_ != _trigger){
		isTimerWorking_ = _trigger;
		emit TimerWorkingStateChanged(isTimerWorking_);
	}
}

bool Timer::IsSessionOngoing() const noexcept{
	return isSessionOngoing_;
}

void Timer::SessionOngoing(bool _state) {
	if(isSessionOngoing_ != _state){
		isSessionOngoing_ = _state;
		emit TimeStartStatusChanged();
	}
}

unsigned int Timer::MaxPauseDurationSec() const noexcept{
	return maxPauseDurationSec_;
}

void Timer::MaxPauseDurationSec(unsigned int _durationSec) {
	if(_durationSec < 1)
		_durationSec = 1;
	if(_durationSec > 3600)
		_durationSec = 3600;
	if(maxPauseDurationSec_ != _durationSec){
		maxPauseDurationSec_ = _durationSec;
		emit MaxPauseDurationSecChanged(_durationSec);
		qDebug() << "Maximum time break duration changed: " << _durationSec << "sec";
	}
}

void Timer::SetTimePoint() {
	if(!isTimerWorking_)
		return;
	auto now = time_point_cast<seconds>(system_clock::now());
	duration<double> pauseDuration = now - lastActive_;
	if(pauseDuration.count() <= maxPauseDurationSec_){
		durationWritingCode_ += pauseDuration;
		emit TimeWritingCodeChanged();
	}
	lastActive_ = now;
}

void Timer::CountWorkingTime() {
	auto now = time_point_cast<seconds>(system_clock::now());
	duration<double> workDuration = now - startWork_;
	durationTimeWorking_ += workDuration;
}

void Timer::Start() {
	controller_.get().SysTrayWidget().SetActionEnable("Start", false);
	controller_.get().SysTrayWidget().SetActionEnable("Pause", true);
	controller_.get().SysTrayWidget().SetTrayIcon("start_standart");
	if(!buttonPausePressed_){  // fresh start
		qDebug() << "Timer start";

		timerData_.Reset();
		controller_.get().EventFilter().SetWindowsHook();
		durationTimeWorking_ = duration<double>(0);
		emit TimeStartChanged();
	} else { // start after pause
		qDebug() << "Timer resume";
		buttonPausePressed_ = false;
	}
	startWork_ = time_point_cast<seconds>(system_clock::now());
	lastActive_ = startWork_;
	FillTimerData();
	TimerWorking(true);
	SessionOngoing(true);
}

void Timer::Pause() {
	qDebug() << "Timer pause";
	buttonPausePressed_ = true;
	CountWorkingTime();
	controller_.get().SysTrayWidget().SetActionEnable("Start", true);
	controller_.get().SysTrayWidget().SetActionEnable("Pause", false);
	controller_.get().SysTrayWidget().SetTrayIcon("pause");
	TimerWorking(false);
}

void Timer::Stop() {
	qDebug() << "Timer stop";
	controller_.get().EventFilter().UnhookWindowsHook();
	controller_.get().SysTrayWidget().SetActionEnable("Start", true);
	controller_.get().SysTrayWidget().SetActionEnable("Pause", false);
	controller_.get().SysTrayWidget().SetTrayIcon("stop");
	TimerWorking(false);
	if(!buttonPausePressed_){
		CountWorkingTime();
	 }
	buttonPausePressed_ = false;
	SessionOngoing(false);
	RecordTimerData();
	QTimer::singleShot(1500, this, [this](){
								controller_.get().SysTrayWidget().SetTrayIcon("main");
								});
}

void Timer::FillTimerData() {
	timerData_.WorkSeconds((int32_t)durationTimeWorking_.count());
	timerData_.WritingCodeSeconds((int32_t)durationWritingCode_.count());
	timerData_.EndTime(QTime::currentTime());
}

void Timer::RecordTimerData() {
	FillTimerData();
	controller_.get().DB().InsertIntoTable(timerData_);
	durationWritingCode_ = duration<double>(0);
	emit TimeWritingCodeChanged();
}

}//namespace NS_Timer
