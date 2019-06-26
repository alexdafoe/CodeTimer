#ifndef LOG_H
#define LOG_H
#include <QObject>
#include <QtMsgHandler>
#include <QString>
#include <QFile>
#include <QDateTime>

class Controller;

// Message handler
void messageToFile(QtMsgType                        type,
                          const QMessageLogContext& context,
                          const QString&                    msg);

// This class is a file manager for log
class Log : public QObject
{
    Q_OBJECT
public:
    // Constructor
    Log(QObject* parent, Controller* controller);
    // Destructor
    virtual ~Log();
    // Initialization method
    void initLogFile(const QString &path);

    // Full log file name. Propagated to QML
    Q_PROPERTY(QString logFileName READ getLogFileName CONSTANT)
    QString getLogFileName() const;

public slots:
    // Remove and create new log file. Propagated to QML
    void removeCurrentLogFile();
    // Open log file in standard windows application. Propagated to QML
    void openLogFileFolder();

public:
    // Public static variable for message handler
    static QDateTime _logTime;
    static QFile currentLogFile;

private:
    Controller* _controller;
    QString _logFileName;
};
#endif // LOG_H
