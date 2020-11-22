#include "database.h"
#include "databasecontroller.h"
#include "timerdata.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QFile>
#include <QDate>
#include <QDebug>

namespace NS_Timer
{

DataBase::DataBase(QObject* _parent, DataBaseController* _dbController)
: QObject(_parent)
, dbController_(_dbController)
{}

DataBase::~DataBase() {
	this->CloseDB();
}

void DataBase::ConnectToDB() {
	QFile dbFile;
	QString path;
	dbController_->GetPath(path);
	dbFile.setFileName(QString(path + DATABASE_NAME));
	if(!dbFile.exists()){
		this->RestroreDB();
	} else {
		this->OpenDB();
	}
}

void DataBase::CloseDB() {
	if(db_.isOpen()){
		qDebug() << "close Database";
		db_.close();
	}
}

QString DataBase::Note() const {
	return note_;
}

void DataBase::Note(const QString& _note) {
	if(note_ != _note){
		note_ = _note;
		emit noteChanged(_note);
	}
}

bool DataBase::InsertIntoTable(const TimerData& _data) {
	qDebug() << "check time for record data";
	if(_data.SecondsWritingCode() <= 0)
		return false;

	QSqlQuery query(db_);
	if(query.prepare("INSERT INTO " TABLE_NAME " ( " _DATE ", "
					 _START_TIME ", " _END_TIME ", "
					 _WORK_TIME ", " _WRITING_CODE_TIME ", "
					 _WORK_SECS ", " _WRITING_CODE_SECS ", "
					 _COMMENTS ") "
					 "VALUES (:day, :start, :end, :worktime, :codetime, :worksecs, :codesecs, :note);"
					 )){
		query.bindValue(":day", _data.DateStart());
		query.bindValue(":start", _data.TimeStart());
		query.bindValue(":end", _data.TimeEnd());
		query.bindValue(":worktime", _data.TimeWork());
		query.bindValue(":codetime", _data.TimeWritingCode());
		query.bindValue(":worksecs", _data.SecondsWork());
		query.bindValue(":codesecs", _data.SecondsWritingCode());
		query.bindValue(":note", note_);
	} else {
		qDebug() << "SQL Prepare failed: insert TimerData into table";
		return false;
	}
	if(!query.exec()){
		qDebug() << "Error insert into " << TABLE_NAME << query.lastError().text();
		return false;
	}
	qDebug() << "Successfully recorded statistic data";
	dbController_->UpdateModel();
	return true;
}

bool DataBase::EditNoteById(int _id, const QString& _newNote) {
	qDebug() << "edit record id: " << _id << " with new note: " << _newNote;
	QSqlQuery query(db_);
	if(query.prepare("UPDATE " TABLE_NAME " SET " _COMMENTS " = :newNote WHERE id = :ID ;")){
		query.bindValue(":newNote", _newNote);
		query.bindValue(":ID", _id);
	} else {
		qDebug() << "SQL Prepare edit note failed!";
		return false;
	}
	if(!query.exec()){
		qDebug() << "Error edit note with id:" << _id << query.lastError().text();
		return false;
	}
	return true;
}

bool DataBase::CollapseDateByNote(const QDate& _date, const QString& _note) {
	bool sqlState = true;
	QVector<int> idForDelete;
	QVariantList listOfKeys;
	int idUpdate = this->GetIdUpdateDate(_date, listOfKeys, idForDelete);
	if(idUpdate < 0)
		sqlState = false;

	db_.transaction();
	while(sqlState){ // instead of enclosed "if"
		QSqlQuery queryUpd(db_);
		sqlState = queryUpd.prepare("UPDATE " TABLE_NAME " SET "
							_START_TIME " = :startSessionStr, "
							_END_TIME " = :endSessionStr , "
							_WORK_TIME " = :workTime , "
							_WRITING_CODE_TIME " = :writingCodeTime , "
							_WORK_SECS " = :workSecs , "
							_WRITING_CODE_SECS " = :writingCodeSecs , "
							_COMMENTS " = :note "
							"WHERE id = :ID ;"
							);
		if(!sqlState){
			qDebug() << "SQL Prepare failed: Unite date with note";
			break;
		}
		queryUpd.bindValue(":startSessionStr", listOfKeys[0].toString());
		queryUpd.bindValue(":endSessionStr", listOfKeys[1].toString());
		queryUpd.bindValue(":workTime", listOfKeys[2].toString());
		queryUpd.bindValue(":writingCodeTime", listOfKeys[3].toString());
		queryUpd.bindValue(":workSecs", listOfKeys[4].toInt());
		queryUpd.bindValue(":writingCodeSecs", listOfKeys[5].toInt());
		queryUpd.bindValue(":note", _note);
		queryUpd.bindValue(":ID", idUpdate);

		sqlState = queryUpd.exec();
		if(!sqlState){
			qDebug() << "Database error: can't Unite date" << queryUpd.lastError().text();
			break;
		}
		break;
	}

	if(sqlState){
		sqlState = db_.commit();
	}
	if(!sqlState){
		qDebug() << "ERROR transaction: Unite date" << _date.toString("yy.MM.dd") << " with note" << _note;
		db_.rollback();
	} else {
		qDebug() << "Success transaction: Unite date" << _date.toString("yy.MM.dd") << " with note" << _note;
		dbController_->ClearSelectedIdList();
		if(!idForDelete.isEmpty()){
			for(auto id : idForDelete)
				DeleteRecordById(id);
		}
	}
	return sqlState;
}

bool DataBase::DeleteRecordById(int _id) {
	QSqlQuery query(db_);
	if(query.prepare("DELETE FROM " TABLE_NAME " WHERE id = :ID ;")){
		query.bindValue(":ID", _id);
	} else {
		qDebug() << "SQL Prepare failed: delete record with id:" <<  _id;
		return false;
	}

	if(!query.exec()){
		qDebug() << "Error delete row " << TABLE_NAME << query.lastError().text();
		return false;
	}
	qDebug() << "Successfully removed record with id: "<< _id;
	return true;
}

bool DataBase::DeleteAllRecords() {
	QSqlQuery lastQuery = dbController_->CurrentQuery();
	QString queryStr = lastQuery.executedQuery();
	if(queryStr.contains("WHERE")){
		return RemoveRecordsInLastQuery(lastQuery);
	}
	QSqlQuery query(db_);
	if(!query.prepare("DELETE FROM " TABLE_NAME " ;")){
		qDebug() << "SQL Prepare failed: delete all records";
		return false;
	}
	if(!query.exec()){
		qDebug() << "Database error: can't remove all records from " << TABLE_NAME << query.lastError().text();
		return false;
	}
	qDebug() << "Successfully removed all records from statistic table";
	return true;
}

bool DataBase::RemoveRecordsInLastQuery(const QSqlQuery& _lastQuery) {
	QStringList keys, values;
	QString newQueryStr = this->PrepareQueryStrToBind(_lastQuery, keys, values);
	newQueryStr.insert(0, "DELETE FROM " TABLE_NAME " WHERE ");

	QSqlQuery query(db_);
	if(query.prepare(newQueryStr)){
		for(int i = 0; i < values.count(); ++i){
			query.bindValue(keys.at(i), values.at(i));
		}
	} else {
		qDebug() << "SQL Prepare failed: delete all records from current table";
		return false;
	}

	if(!query.exec()){
		qDebug() << "Database error: can't remove all records from current table " << query.lastError().text();
		qDebug() << "Error query: " << query.lastQuery();
		return false;
	}
	return true;
}

QString DataBase::NoteById(int _id) {
	QSqlQuery query(db_);
	QString note;
	if(query.prepare("SELECT " _COMMENTS " FROM " TABLE_NAME " WHERE id = :ID ;")){
		query.bindValue(":ID", _id);
	} else {
		qDebug() << "SQL Prepare failed: get note from id " << _id;
		return note;
	}
	if(query.exec()){
		if(query.next()){
			note = query.value(_COMMENTS).toString();
		}
	}
	return note;
}

bool DataBase::OpenDB() {
	qDebug() << "open Database";
	db_ = QSqlDatabase::addDatabase("QSQLITE");
	db_.setHostName(DATABASE_HOSTNAME);
	QString path;
	dbController_->GetPath(path);
	db_.setDatabaseName(path + DATABASE_NAME);
	return db_.open();
}

bool DataBase::RestroreDB() {
	if(this->OpenDB())
		return this->CreateTable();
	qWarning() << "Can't restore database";
	return false;
}

bool DataBase::CreateTable() {
	QSqlQuery query(db_);
	if(query.exec("CREATE TABLE " TABLE_NAME " ("
				  "id INTEGER PRIMARY KEY, "
				  _DATE " DATE NOT NULL, "
				  _START_TIME " VARCHAR2(20), "
				  _END_TIME " VARCHAR2(20), "
				  _WORK_TIME " VARCHAR2(20), "
				  _WRITING_CODE_TIME " VARCHAR2(20), "
				  _WORK_SECS " INTEGER NOT NULL, "
				  _WRITING_CODE_SECS " INTEGER NOT NULL, "
				  _COMMENTS " VARCHAR2(30)"
				  ");" )) {
		return true;
	}
	qDebug() << "DataBase error: can't create table \'" << TABLE_NAME << "\' ";
	qDebug() << query.lastError().text();
	return false;
}

QString DataBase::PrepareQueryStrToBind(const QSqlQuery& _query, QStringList& _keys, QStringList& _values) {
	QString queryStr = _query.executedQuery();
	QMapIterator<QString, QVariant> it(_query.boundValues());
	QStringList fields;
	while(it.hasNext()) { // parse query by keys and values
		it.next();
		_keys.append(it.key());
		_values.append(it.value().toString());
	}
	// cut out everything before 'where' stmt
	int pos = queryStr.indexOf("WHERE ");
	if(pos >= 0){
		queryStr.remove(0, pos + 6);
		queryStr.remove(QChar(';'), Qt::CaseInsensitive);
	}
	QStringList strListFields = queryStr.split(':'); // split by binded values (field = :key)
	strListFields.pop_back(); // erase last key

	for(int i = 1; i <= _values.count(); ++i){ // fill fields list
		QString str = strListFields.at(i-1);
		if(i % 2 == 0) { // erase 'key'
			int p = str.indexOf(' ');
			str.remove(0, p);
		}
		fields.append(str);
	}
	QString newQueryStr;
	for(int i = 0; i < _values.count(); ++i){
		newQueryStr.append(fields.at(i));
		newQueryStr.append(_keys.at(i));
	}
	return newQueryStr;
}

int DataBase::GetIdUpdateDate(const QDate& _date, QVariantList& _listOfKeys, QVector<int>& _trashId) {
	QVariantList idList;
	dbController_->GetSelectedIdList(idList);

	if(idList.isEmpty())
		dbController_->SearchDate(_date);
	else
		dbController_->SearchDateUsingIdList(_date);

	QSqlQuery query = dbController_->CurrentQuery();
	QString startTmpStr, endTmpStr;
	QTime startSessionTime, startTmpTime;
	QTime endSessionTime, endTmpTime;
	int workSecs = 0;
	int writingCodeSecs = 0;
	int idUpd = -1;
	_listOfKeys.clear();
	// Reserve for list 6 field:
	// 0 - startSession, 1 - endSession, 2 - workTime, 3 - codeTime, 4 -workSec, 5 - codeSec
	_listOfKeys.reserve(6);
	if(query.exec()){
		if(query.next()){
			idUpd = query.value("id").toInt();
			_listOfKeys.append(query.value(_START_TIME).toString()); // listOfKeys[0]
			startSessionTime = QTime::fromString(_listOfKeys[0].toString(), "hh:mm:ss");
			endTmpStr = query.value(_END_TIME).toString();
			_listOfKeys.append(endTmpStr); // listOfKeys[1]
			endTmpTime = QTime::fromString(endTmpStr, "hh:mm:ss");
			workSecs = query.value(_WORK_SECS).toInt();
			writingCodeSecs = query.value(_WRITING_CODE_SECS).toInt();
		}
		while(query.next()){
			_trashId.append(query.value("id").toInt());
			startTmpStr = query.value(_START_TIME).toString();
			startTmpTime = QTime::fromString(startTmpStr, "hh:mm:ss");
			_listOfKeys[1] = query.value(_END_TIME).toString();
			endSessionTime = QTime::fromString(_listOfKeys[1].toString(), "hh:mm:ss");
			workSecs += query.value(_WORK_SECS).toInt();
			writingCodeSecs += query.value(_WRITING_CODE_SECS).toInt();
			if(startTmpTime < startSessionTime){ // make sure that start session time is correct
				_listOfKeys[0] = startTmpStr;
			}
			if(endTmpTime > endSessionTime){ // make sure that end session time is correct
				_listOfKeys[1] = endTmpStr;
			}
		}
		QString workTime, writingCodeTime;
		TimeSecToString()(workSecs, workTime);
		TimeSecToString()(writingCodeSecs, writingCodeTime);

		_listOfKeys.append(workTime); // listOfKeys[2]
		_listOfKeys.append(writingCodeTime); // listOfKeys[3]
		_listOfKeys.append(workSecs); // listOfKeys[4]
		_listOfKeys.append(writingCodeSecs); // listOfKeys[5]
	}
	return idUpd;
}

}//namespace NS_Timer
