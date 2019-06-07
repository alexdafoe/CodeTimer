#include "include/timerdata.h"

TimerData::TimerData(QDateTime &&startDateTime) {
    if(startDateTime.isValid()){
        _startDateTime = startDateTime;
        _endTime = startDateTime.time();
    }
}

void TimerData::setWorkSeconds(const int &sec){
    if(sec > 0)
        _workSec = sec;
}

void TimerData::setWritingCodeSeconds(const int &sec) {
    if(sec > 0)
        _writingCodeSec = sec;
}

void TimerData::setEndTime(const QTime &time) {
    if(time > _endTime && time.isValid())
        _endTime = time;
}

QDate TimerData::getDateStart() const {
    return _startDateTime.date();
}

QString TimerData::getTimeStart() const {
    return _startDateTime.time().toString("hh:mm:ss");
}

QString TimerData::getTimeEnd() const {
    return _endTime.toString("hh:mm:ss");
}

QString TimerData::getTimeWork() const {
    QString workTimeStr{"00:00:00"};
    if(_workSec > 0)
        TimeSecToString()(_workSec, workTimeStr);
    return workTimeStr;
}

QString TimerData::getTimeWritingCode() const {
    QString writingCodeTimeStr{"00:00:00"};
    if(_writingCodeSec > 0)
        TimeSecToString()(_writingCodeSec, writingCodeTimeStr);
    return writingCodeTimeStr;
}

int TimerData::getSecondsWork() const noexcept{
    return _workSec;
}

int TimerData::getSecondsWritingCode() const noexcept{
    return  _writingCodeSec;
}

