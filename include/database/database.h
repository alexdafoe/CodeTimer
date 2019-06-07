#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>

#define DATABASE_NAME        "statistic.db"
#define DATABASE_HOSTNAME "DataStat"
#define TABLE_NAME              "dailystat"
#define _DATE                       "session_date"
#define _START_TIME              "session_start"
#define _END_TIME                 "session_end"
#define _WORK_TIME              "work_time"
#define _WRITING_CODE_TIME "writing_code_time"
#define _WORK_SECS              "work_sec"
#define _WRITING_CODE_SECS "writing_code_sec"
#define _COMMENTS               "comments"


class DataBaseController;
class TimerData;

class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase(QObject *parent, DataBaseController* dbController);
    virtual ~DataBase();

    void connectToDataBase();
    void closeDataBase();

    Q_PROPERTY(QString note READ getNote WRITE setNote NOTIFY noteChanged)
    QString getNote() const;
    void setNote(const QString& note);

signals:
    void noteChanged(QString);

public slots:
    bool insertIntoTable(const TimerData *data);

    bool editNote(const int id, const QString& newNote);
    bool uniteDateWithNote(const QDate &date, const QString& note);
    bool removeRecordById(const int id);
    bool removeAllRecords();

    QString getNoteById(const int id);

private:
    bool openDataBase();
    bool restroreDataBase();
    bool createTable();
    bool removeRecordsFromLastQuery(const QSqlQuery& lastQuery);
    QString getStrQueryToBind(const QSqlQuery& query, QStringList& keys, QStringList& values);
    int getIdUpdateDate(const QDate &date, QVariantList& listOfKeys, QVector<int>& trashId);

private:
    DataBaseController* _dbController;
    QSqlDatabase db;
    QString _note;
};

#endif // DATABASE_H
