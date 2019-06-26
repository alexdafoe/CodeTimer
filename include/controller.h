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

// This class is mediator between different classes
// Provide access for QML layer
// Transfers messages between Timer and database
class Controller : public QObject
{
    Q_OBJECT
public:
    // Constructor
    explicit Controller(QObject *parent = nullptr);
    // Destructor
    virtual ~Controller();

    // Initialization method
    void init();

    SymbolsSettings* getSymbolsSettings() const;
    Timer* getTimer() const;
    TrayIconWidget* getSysTrayWidget() const;
    DataBaseController* getDataBaseController() const;
    Log* getLog() const;
    KeyEventFilter* getEventFilter() const;

    // Returns a list of key symbols which enabled for tracking
    void getSymbolsList(QList<unsigned long> &list) const;
     // Returns path where aloceted database and log files
    void getDirectoryPath(QString &path) const noexcept;
    bool isTimerWorking() const noexcept;

    // Sends enable windows hook for tracking keyboard
    void sendSetWindowsHook() const;
    // Sends disable windows hook for tracking keyboard
    void sendUnhookWindowsHook() const;
    // Sends set key symbols for tracking
    void sendSetSymbolListIntoFilter(const QList<unsigned long> &list) const;
    // Sends event that special key detected
    void sendControlKeyNoticed();
    // Sends event insert TimerData to database
    void sendRecordData(const TimerData *data);
    // Sends set activate/deactivate item in tray widget menu
    void sendSetTrayMenuActionEnable(const QString &actionName, bool isEnable);
    // Sends set tray widget icon
    void sendSetTrayIcon(const QString &icon_name);

public slots:
    // Sends events to Timer
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
    QString _appPath; // path for database and log files
};

#endif // CONTROLLER_H
