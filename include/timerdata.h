#ifndef TIMERDATA_H
#define TIMERDATA_H

#include "include/timesectostring.h"
#include <QDateTime>

// This class is a stores of all timer data in current session
// Used for sends to database for record in table
class TimerData
{
public:
    // Constructor
    explicit TimerData(QDateTime &&startDateTime = QDateTime::currentDateTime());
    TimerData& operator=(const TimerData&) = default;
    TimerData& operator=(TimerData&&) = default;
    // Destructor
    virtual ~TimerData() = default;

    // Set data from timer
    void setWorkSeconds(const int& sec);
    void setWritingCodeSeconds(const int& sec);
    void setEndTime(const QTime& time);

    // Get data for the database
    QDate getDateStart() const;
    QString getTimeStart() const;
    QString getTimeEnd() const;
    QString getTimeWork() const;
    QString getTimeWritingCode() const;
    int getSecondsWork() const noexcept;
    int getSecondsWritingCode() const noexcept;

private:
    int _workSec{0};
    int _writingCodeSec{0};
    QDateTime _startDateTime;
    QTime _endTime;
};

#endif // TIMERDATA_H
