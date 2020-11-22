#include "timer.h"
#include "controller.h"
#include "timesectostring.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

using namespace std::chrono;

Timer::Timer(QObject *parent, Controller* controller):
	QObject(parent),
	_controller(controller)
{
	// refresh clock every 1 sec (main.qml onTimerTrigger)
	_trigger.start(1000);
	QObject::connect(&_trigger, SIGNAL(timeout()), this, SIGNAL(timerTrigger()));
	_timeWritingCode = duration<double>(0);
}

Timer::~Timer() {
	if(_isSessionOngoing) stop(); //even if on pause
}

QString Timer::getTimeStart() const {
	if(_timerData != nullptr)
		return _timerData->getTimeStart();
	return QString();
}

QString Timer::getTimerStr() {
	auto now_sec = time_point_cast<seconds>(system_clock::now());
	auto realTime = now_sec - _lastActive;
	double time = _timeWritingCode.count();
	if(_isTimerWorking){
		// SEC_DIVIDER is a compiler dependent variable
		// calculation std::chrono::duration to seconds
		// established in .pro file
		time += realTime.count()/SEC_DIVIDER;
	}
	TimeSecToString()(int(time), _timerStr);
	return _timerStr;
}

QString Timer::getTimeWritingCodeStr() {
	TimeSecToString()(int(_timeWritingCode.count()), _timeLeftStr);
	return _timeLeftStr;
}

bool Timer::isTimerWorking() const noexcept{
	return _isTimerWorking;
}

void Timer::setTimerWorking(bool trigger) {
	if(_isTimerWorking != trigger){
		_isTimerWorking = trigger;
		emit timerWorkingChanged(_isTimerWorking);
	}
}

bool Timer::isSessionOngoing() const noexcept{
	return _isSessionOngoing;
}

void Timer::setSessionOngoing(bool state) {
	if(_isSessionOngoing != state){
		_isSessionOngoing = state;
		emit timeStartStatusChanged();
	}
}

unsigned int Timer::getMaxSecPauseDuration() const noexcept{
	return _maxSecPauseDuration;
}

void Timer::setMaxSecPauseDuration(unsigned int durationSec) {
	if(durationSec < 1)
		durationSec = 1;
	if(durationSec > 3600)
		durationSec = 3600;
	if(_maxSecPauseDuration != durationSec){
		_maxSecPauseDuration = durationSec;
		emit maxSecPauseDurationChanged(durationSec);
		qDebug() << "Maximum time break duration changed: " << durationSec << "sec";
	}
}

void Timer::setTimePoint() {
	if(!_isTimerWorking)
		return;
	auto now_sec = time_point_cast<seconds>(system_clock::now());
	duration<double> pauseDuration = now_sec - _lastActive;
	if(pauseDuration.count() <= _maxSecPauseDuration){
		_timeWritingCode += pauseDuration;
		emit timeWritingCodeChanged();
	}
	_lastActive = now_sec;
}

void Timer::countWorkingTime() {
	auto now_sec = time_point_cast<seconds>(system_clock::now());
	duration<double> workDuration = now_sec - _startWork;
	_timeWorking += workDuration;
}

void Timer::start() {
	_controller->sendSetTrayMenuActionEnable("Start", false);
	_controller->sendSetTrayMenuActionEnable("Pause", true);
	_controller->sendSetTrayIcon("start_standart");
	if(!_buttonPausePressed){  // fresh start
		qDebug() << "Timer start";
		try {
			_timerData.reset(new TimerData());
		} catch (std::exception ex) {
			qWarning() << ex.what();
			QMessageBox msg;
			msg.setIcon(QMessageBox::Critical);
			msg.setText(ex.what());
			msg.setStandardButtons(QMessageBox::Ok);
			msg.exec();
			this->stop();
			return;
		}
		_controller->sendSetWindowsHook();
		_timeWorking = duration<double>(0);
		emit timeStartChanged();
	} else { // start after pause
		qDebug() << "Timer resume";
		_buttonPausePressed = false;
	}
	_startWork = time_point_cast<seconds>(system_clock::now());
	_lastActive = _startWork;
	this->fillTimerData();
	setTimerWorking(true);
	setSessionOngoing(true);
}

void Timer::pause() {
	qDebug() << "Timer pause";
	_buttonPausePressed = true;
	countWorkingTime();
	_controller->sendSetTrayMenuActionEnable("Start", true);
	_controller->sendSetTrayMenuActionEnable("Pause", false);
	_controller->sendSetTrayIcon("pause");
	setTimerWorking(false);
}

void Timer::stop() {
	qDebug() << "Timer stop";
	_controller->sendUnhookWindowsHook();
	_controller->sendSetTrayMenuActionEnable("Start", true);
	_controller->sendSetTrayMenuActionEnable("Pause", false);
	_controller->sendSetTrayIcon("stop");
	setTimerWorking(false);
	if(!_buttonPausePressed){
		countWorkingTime();
	 }
	_buttonPausePressed = false;
	setSessionOngoing(false);
	recordTimerData();
	QTimer::singleShot(1500, this, [this](){
								_controller->sendSetTrayIcon("main");
								});
}

void Timer::fillTimerData() {
	if(_timerData != nullptr) {
		_timerData->setWorkSeconds(int(_timeWorking.count()));
		_timerData->setWritingCodeSeconds(int(_timeWritingCode.count()));
		_timerData->setEndTime(QTime::currentTime());
	}
}

void Timer::recordTimerData() {
	this->fillTimerData();
	_controller->sendRecordData(_timerData.get());
	_timeWritingCode = duration<double>(0);
	emit timeWritingCodeChanged();
}
