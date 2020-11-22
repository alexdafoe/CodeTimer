#pragma once
#include <QObject>
#include <QSql>
#include <QSqlDatabase>

// Define macro names
#define DATABASE_NAME			"statistic.db"
#define DATABASE_HOSTNAME	"DataStat"
#define TABLE_NAME				"dailystat"
#define _DATE							"session_date"
#define _START_TIME				"session_start"
#define _END_TIME					"session_end"
#define _WORK_TIME				"work_time"
#define _WRITING_CODE_TIME	"writing_code_time"
#define _WORK_SECS				"work_sec"
#define _WRITING_CODE_SECS	"writing_code_sec"
#define _COMMENTS					"comments"

class DataBaseController;
class TimerData;

// This class is a SQLite database manager
// Provide necessary work operations with database
class DataBase : public QObject
{
	Q_OBJECT
public:
	DataBase(QObject *parent, DataBaseController* dbController);
	virtual ~DataBase();

	void			connectToDataBase();
	void			closeDataBase();

	// The note for current session. Propagated to QML
	Q_PROPERTY(QString note READ getNote WRITE setNote NOTIFY noteChanged)
	QString		getNote() const;
	void			setNote(const QString& note);

	// Query insert TimerData into table in database
	bool			insertIntoTable(const TimerData *data);

signals:
	void			noteChanged(QString);

public slots:
	// Propagated to QML

	// Query update note in row by id
	bool			editNote(const int id, const QString& newNote);
	// Query update row by date using note, if more than 1 rows - unite them
	bool			uniteDateWithNote(const QDate &date, const QString& note);
	bool			deleteRecordById(const int id);
	bool			deleteAllRecords();
	QString		getNoteById(const int id);

private:
	bool			openDataBase();
	bool			restroreDataBase();
	bool			createTable();
	bool			removeRecordsFromLastQuery(const QSqlQuery& lastQuery);
	// Returns last query in string and parse keys and values into list's
	QString		getStrQueryToBind(const QSqlQuery& query,QStringList& keys, QStringList& values);
	// Returns id for unite in 1 row and cache all values from rows that day
	int				getIdUpdateDate(const QDate &date, QVariantList& listOfKeys, QVector<int>& trashId);

private:
	DataBaseController*	_dbController;
	QSqlDatabase			db;
	QString					_note;
};
