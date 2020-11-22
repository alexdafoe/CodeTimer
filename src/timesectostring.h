#pragma once
#include <QString>

namespace NS_Timer
{

// Convert time from seconds to string for clock and database
class TimeSecToString
{
public:
	void operator()(int _time, QString& _outStr){
		_outStr.clear();
		if(_time <= 0){
			_outStr = "00:00:00";
			return;
		}
		int timeFromSec[3];
		timeFromSec[0] = 0; // hours
		timeFromSec[1] = 0; // minutes
		timeFromSec[2] = _time; // seconds

		while(timeFromSec[2] >= 3600){
			timeFromSec[0]++; // increace hours
			timeFromSec[2] -= 3600; // dicreace seconds
		}
		while(timeFromSec[2] >= 60){
			timeFromSec[1]++; // increace minutes
			timeFromSec[2] -= 60; // dicreace seconds
		}
		for(size_t i = 0; i < 3; ++i){ // create view like a "00:00:00"
			if(timeFromSec[i] < 10){
				_outStr.append(QString("0%1:").arg(timeFromSec[i]));
			} else {
				_outStr.append(QString("%1:").arg(timeFromSec[i]));
			}
		}
		_outStr.remove(_outStr.length()-1, 1); // remove last colon
	}
};

}//namespace NS_Timer
