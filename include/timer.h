#ifndef TIMER_H
#define TIMER_H
#include "include/timerdata.h"

#include <QObject>
#include <QTimer>
#include <chrono>
#include <ctime>
#include <memory>

class Controller;

class Timer : public QObject
{
    Q_OBJECT

public:
    explicit Timer(QObject* parent = nullptr, Controller* controller = nullptr);
    virtual ~Timer();

    Q_PROPERTY(QString timeStartStr READ getTimeStart NOTIFY timeStartChanged)
    QString getTimeStart() const;

    Q_PROPERTY(QString timerStr READ getTimerStr CONSTANT)
    QString getTimerStr();

    Q_PROPERTY(QString timeWritingCodeStr READ getTimeWritingCodeStr CONSTANT)
    QString getTimeWritingCodeStr();

    Q_PROPERTY(bool isTimerWorking READ isTimerWorking WRITE setTimerWorking NOTIFY timerWorkingChanged)
    bool isTimerWorking() const noexcept;
    void setTimerWorking(bool trigger);

    Q_PROPERTY(bool isSessionOngoing READ isSessionOngoing WRITE setSessionOngoing NOTIFY timeStartStatusChanged)
    bool isSessionOngoing() const noexcept;
    void setSessionOngoing(bool state);

    Q_PROPERTY(unsigned int maxSecPauseDuration READ getMaxSecPauseDuration WRITE setMaxSecPauseDuration NOTIFY maxSecPauseDurationChanged)
    unsigned int getMaxSecPauseDuration() const noexcept;
    void setMaxSecPauseDuration(unsigned int durationSec);

    void setTimePoint();
    void countWorkingTime();

public slots:
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
    void recordTimerData();
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
    QTimer _trigger;
    QString _timerStr{"0"};
    QString _timeLeftStr;
};

#endif // TIMER_H
