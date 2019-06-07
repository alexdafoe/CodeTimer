#ifndef TIMESECTOSTRING_H
#define TIMESECTOSTRING_H

#include <QString>

class TimeSecToString {

public:
    void operator()(const int &time, QString &str){
        str.clear();
        if(time <= 0){
            str = "00:00:00";
            return;
        }
        int timeFromSec[3];
        timeFromSec[0] = 0; // hours
        timeFromSec[1] = 0; // minutes
        timeFromSec[2] = time; // seconds

        while(timeFromSec[2] >= 3600){
            timeFromSec[0]++; // increace hours
            timeFromSec[2] -= 3600; // dicreace seconds
        }
        while(timeFromSec[2] >= 60){
            timeFromSec[1]++; // increace minutes
            timeFromSec[2] -= 60; // dicreace seconds
        }
        for(size_t i = 0; i < 3; ++i){
            if(timeFromSec[i] < 10){
                str.append(QString("0%1:").arg(timeFromSec[i]));
            } else {
                str.append(QString("%1:").arg(timeFromSec[i]));
            }
        }
        str.remove(str.length()-1, 1); // remove last double dots
    }

};

#endif // TIMESECTOSTRING_H
