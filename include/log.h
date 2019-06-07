#ifndef LOG_H
#define LOG_H
#include <QObject>
#include <QtMsgHandler>
#include <QString>
#include <QFile>
#include <QDateTime>

class Controller;

class Log : public QObject{
    Q_OBJECT

public:
    Log(QObject* parent, Controller* controller);
    virtual ~Log();
    void initLogFile(const QString &path);

    Q_PROPERTY(QString logFileName READ getLogFileName CONSTANT)
    QString getLogFileName() const;

public slots:
    void removeCurrentLogFile();
    void openLogFileFolder();

public:
    static QDateTime _logTime;
    static QFile currentLogFile;

private:
    Controller* _controller;
    QString _logFileName;
};

void messageToFile(QtMsgType                        type,
                          const QMessageLogContext& context,
                          const QString&                    msg);

#endif // LOG_H
