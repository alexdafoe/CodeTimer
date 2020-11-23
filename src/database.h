#pragma once
#include "timerdata.h"
#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <memory>

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

namespace NS_Timer
{

class DatabaseModel;

// This class is a SQLite database manager
// Provide necessary work operations with database
class DatabaseWrap : public QObject
{
	Q_OBJECT
public:
	DatabaseWrap(std::shared_ptr<DatabaseModel>, const QString& workDirictory);
	DatabaseWrap(const DatabaseWrap&)												= delete;
	DatabaseWrap(const DatabaseWrap&&)											= delete;
	virtual ~DatabaseWrap();

	DatabaseWrap&	operator=(const DatabaseWrap&)							= delete;
	DatabaseWrap&	operator=(const DatabaseWrap&&)							= delete;

	void					ConnectToDB();
	void					CloseDB();

	// The note for current session. Propagated to QML
	Q_PROPERTY(QString	note_
						READ		Note
						WRITE	Note
						NOTIFY	NoteChanged)
	QString				Note() const;
	void					Note(const QString& note);

	// Query insert TimerData into table in database
	bool					InsertIntoTable(const TimerData&);

signals:
	void					NoteChanged(QString);

public slots:
	// Propagated to QML

	// Query update note in row by id
	bool			EditNoteById(int id, const QString& newNote);
	// Query update row by date using note, if more than 1 rows - unite them
	bool			CollapseDateByNote(const QDate& date, const QString& note); //todo: uniteDateWithNote
	bool			DeleteRecordById(int id);
	bool			DeleteAllRecords();
	QString		NoteById(int id);

private:
	bool			OpenDB();
	bool			RestroreDB();
	bool			CreateTable();
	bool			RemoveRecordsInLastQuery(const QSqlQuery& lastQuery);
	// Returns last query in string and parse keys and values into list's
	QString		PrepareQueryStrToBind(const QSqlQuery& query,QStringList& keys, QStringList& values);
	// Returns id for unite in 1 row and cache all values from rows that day
	int				GetIdUpdateDate(const QDate& date, QVariantList& listOfKeys, QVector<int>& trashId);

private:
	std::shared_ptr<DatabaseModel>	dbModel_;
	QString										workDirictory_;
	QSqlDatabase								db_;
	QString										note_;
};

}//namespace NS_Timer
