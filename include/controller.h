#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSharedPointer>

class SymbolsSettings;
class Timer;
class TimerData;
class KeyEventFilter;
class TrayIconWidget;
class DataBaseController;
class Log;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    virtual ~Controller();

    void init();

    SymbolsSettings* getSymbolsSettings() const;
    Timer* getTimer() const;
    TrayIconWidget* getSysTrayWidget() const;
    DataBaseController* getDataBaseController() const;
    Log* getLog() const;
    KeyEventFilter* getEventFilter() const;

    void getSymbolsList(QList<unsigned long> &list) const;
    void getDirectoryPath(QString &path) const noexcept;
    bool isTimerWorking() const noexcept;

    void sendSetWindowsHook() const;
    void sendUnhookWindowsHook() const;
    void sendSetSymbolListIntoFilter(const QList<unsigned long> &list) const;
    void sendControlKeyNoticed();
    void sendRecordData(const TimerData *data);
    void sendSetTrayMenuActionEnable(const QString &actionName, bool isEnable);
    void sendSetTrayIcon(const QString &icon_name);

public slots:
    void sendStartTimer();
    void sendPauseTimer();
    void sendStopTimer();

private:
    QSharedPointer<Log> _log;
    QSharedPointer<SymbolsSettings> _symbolsSettings;
    QSharedPointer<KeyEventFilter> _eventFilter;
    QSharedPointer<DataBaseController> _dataBaseController;
    QSharedPointer<Timer> _timer;
    QSharedPointer<TrayIconWidget> _trayIcon;
    QString _appPath;
};

#endif // CONTROLLER_H
