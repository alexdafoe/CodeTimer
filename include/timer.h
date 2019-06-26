#ifndef TIMER_H
#define TIMER_H
#include "include/timerdata.h"

#include <QObject>
#include <QTimer>
#include <chrono>
#include <ctime>
#include <memory>

class Controller;

// This class is a main Timer
// Count time between start/pause/stop and key symbols detected event
class Timer : public QObject
{
    Q_OBJECT
public:
    // Constructor
    explicit Timer(QObject* parent = nullptr, Controller* controller = nullptr);
    // Destructor
    virtual ~Timer();

    // String that shows start counting of time. Propagated to QML
    Q_PROPERTY(QString timeStartStr READ getTimeStart NOTIFY timeStartChanged)
    QString getTimeStart() const;

    // The main timer. Propagated to QML
    Q_PROPERTY(QString timerStr READ getTimerStr CONSTANT)
    QString getTimerStr();

    // String that shows time of writing code counting by key symbols. Propagated to QML
    Q_PROPERTY(QString timeWritingCodeStr READ getTimeWritingCodeStr CONSTANT)
    QString getTimeWritingCodeStr();

    // State of main timer. Propagated to QML
    Q_PROPERTY(bool isTimerWorking READ isTimerWorking WRITE setTimerWorking NOTIFY timerWorkingChanged)
    bool isTimerWorking() const noexcept;
    void setTimerWorking(bool trigger);

    // State of main timer for correct shows labels in QML layers. Not change on pause. Propagated to QML
    Q_PROPERTY(bool isSessionOngoing READ isSessionOngoing WRITE setSessionOngoing NOTIFY timeStartStatusChanged)
    bool isSessionOngoing() const noexcept;
    void setSessionOngoing(bool state);

    // Maximum seconds between key symbols detection events
    Q_PROPERTY(unsigned int maxSecPauseDuration READ getMaxSecPauseDuration WRITE setMaxSecPauseDuration NOTIFY maxSecPauseDurationChanged)
    unsigned int getMaxSecPauseDuration() const noexcept;
    void setMaxSecPauseDuration(unsigned int durationSec);

    // Event from KeyEventFilter that special key detected. Count time of writing code
    void setTimePoint();
    // Prepare "working time" for record in TimerData
    void countWorkingTime();

public slots:
    // Propagated to QML
    void start();
    void pause();
    void stop();

signals:
    void timerTrigger();
    void maxSecPauseDurationChanged(unsigned int);
    void timerWorkingChanged(bool);
    void timeWritingCodeChanged();
    void timeStartStatusChanged();
    void timeStartChanged();

protected:
    // Prepare TimerData and sends for record in database
    void recordTimerData();
    // Refresh all data in TimerData
    void updateTimerData();

private:
    Controller* _controller;
    std::chrono::time_point<std::chrono::system_clock> _lastActive;
    std::chrono::time_point<std::chrono::system_clock> _startWork;
    std::chrono::duration<double> _timeWritingCode;
    std::chrono::duration<double> _timeWorking;
    std::unique_ptr<TimerData> _timerData{nullptr};
    unsigned int _maxSecPauseDuration{300}; // 5 min
    bool _buttonPausePressed{false};
    bool _isTimerWorking{false};
    bool _isSessionOngoing{false};
    QTimer _trigger; // call signal timerTrigger every 1 sec for refresh clock in QML layer
    QString _timerStr{"0"};
    QString _timeLeftStr;
};

#endif // TIMER_H
