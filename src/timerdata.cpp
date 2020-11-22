#include "timerdata.h"

namespace NS_Timer
{

void TimerData::Reset()
{
	workSec_				= 0;
	writingCodeSec_		= 0;
	startDateTime_		= QDateTime::currentDateTime();
	endTime_				= startDateTime_.time();
}

void TimerData::WorkSeconds(uint32_t _sec) {
	if (_sec > 0)
		workSec_ = _sec;
}

void TimerData::WritingCodeSeconds(uint32_t _sec) {
	if(_sec > 0)
		writingCodeSec_ = _sec;
}

void TimerData::EndTime(const QTime& _time) {
	if(_time > endTime_ && _time.isValid())
		endTime_ = _time;
}

QDate TimerData::DateStart() const {
	return startDateTime_.date();
}

QString TimerData::TimeStart() const {
	return startDateTime_.time().toString("hh:mm:ss");
}

QString TimerData::TimeEnd() const {
	return endTime_.toString("hh:mm:ss");
}

QString TimerData::TimeWork() const {
	QString workTimeStr{"00:00:00"};
	if(workSec_ > 0)
		TimeSecToString()(workSec_, workTimeStr);
	return workTimeStr;
}

QString TimerData::TimeWritingCode() const {
	QString writingCodeTimeStr{"00:00:00"};
	if(writingCodeSec_ > 0)
		TimeSecToString()(writingCodeSec_, writingCodeTimeStr);
	return writingCodeTimeStr;
}

int TimerData::SecondsWork() const noexcept{
	return workSec_;
}

int TimerData::SecondsWritingCode() const noexcept{
	return  writingCodeSec_;
}

}//namespace NS_Timer
