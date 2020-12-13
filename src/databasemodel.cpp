#include "databasemodel.h"
#include "database.h"
#include <QDate>
#include <QDebug>

namespace NS_Timer
{

DatabaseModel::DatabaseModel(QObject* _parent)
: QSqlQueryModel(_parent)
{}

void DatabaseModel::DateHasNote(bool _state) {
	if(dateHasNote_ != _state){
		dateHasNote_ = _state;
		emit dateHasNoteChanged(_state);
	}
}

void DatabaseModel::DateRowsCount(int _count) {
	if(_count >= 0){
		dateRowsCount_ = _count;
		emit dateRowsCountChanged(_count);
	}
}

QVariant DatabaseModel::Data(const QModelIndex& _item, int _role) const {
	int columnId = _role - Qt::UserRole -1;
	QModelIndex modelIndex = this->index(_item.row(), columnId);
	return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QSqlQuery DatabaseModel::CurrentQuery() const {
	return this->query();
}

void DatabaseModel::update() {
	QSqlQuery query;
	if(query.exec("SELECT * FROM " TABLE_NAME ";")){
		this->setQuery(query);
	} else {
		qDebug() << "SQL Error: update model";
	}
}

void DatabaseModel::updateWithLastQuery() {
	QSqlQuery query = this->query();
	if(query.exec()){
		this->setQuery(query);
	} else {
		qDebug() << "SQL Error: update model";
	}
}

QVariantList DatabaseModel::noteByDate(const QDate& _date) {
	QVariantList listOfNote;
	listOfNote.clear();
	int rowsCount = 0;
	SearchDate(_date);
	if(this->query().exec()){
		QString str;
		while(this->query().next()){ // count num of rows
			++rowsCount;
			str = this->query().value(_COMMENTS).toString();
			if(!str.isEmpty() && !listOfNote.contains(str)){ // fill list
				listOfNote.append(str);
			}
		}
	}
	DateHasNote(!listOfNote.isEmpty());
	DateRowsCount(rowsCount);
	return listOfNote;
}

bool DatabaseModel::SearchDate(const QDate& _date) {
	qDebug() << "Try to find Date:" << _date.toString("yyyy-MM-dd");
	QSqlQuery query;
	if(query.prepare("SELECT id, " _DATE ", "
							_START_TIME ", " _END_TIME ", "
							_WORK_TIME ", " _WRITING_CODE_TIME ", "
							_WORK_SECS ", " _WRITING_CODE_SECS ", "
							_COMMENTS
							" FROM " TABLE_NAME
							" WHERE " _DATE " = :searchDate;")){
		query.bindValue(":searchDate", _date);
	} else {
		qDebug() << "SQL Prepare failed: search date";
		return false;
	}
	if(!query.exec()){
		qDebug() << "SQL Error search date!";
		return false;
	}
	this->setQuery(query);
	return true;
}

bool DatabaseModel::SearchDateWithIdList(const QDate& _date) {
	int idListSize = selectedIdList_.count();
	if(idListSize == 0)
		return SearchDate(_date);
	QString queryStr = QString("SELECT id, " _DATE ", "
							   _START_TIME ", " _END_TIME ", "
							   _WORK_TIME ", " _WRITING_CODE_TIME ", "
							   _WORK_SECS ", " _WRITING_CODE_SECS ", "
							   _COMMENTS
							   " FROM " TABLE_NAME
							   " WHERE id IN (");
	QStringList keys;
	// 1st key w/o comma ',' so add this manually
	keys.append(":sId");
	queryStr.append(":sId");
	// other add in cycle
	for(int i = 1; i < idListSize; ++i){
		keys.append(QString(":sId%1").arg(i));
		queryStr.append(QString(", :sId%1").arg(i));
	}
	queryStr.append(");");
	QSqlQuery query;
	if(query.prepare(queryStr)){ // bind values
		for(int j = 0; j < keys.count(); ++j)
			query.bindValue(keys.at(j), selectedIdList_.at(j).toInt());
	} else {
		qDebug() << "SQL Prepare failed: search date using ID list";
		return false;
	}
	if(!query.exec()){
		qDebug() << "SQL Error search date using ID list!";
		return false;
	}
	this->setQuery(query);
	return true;
}

void DatabaseModel::addSelectedId(int _id) {
	if(!selectedIdList_.contains(_id))
		selectedIdList_.append(_id);
}

void DatabaseModel::clearIdList() noexcept{
	selectedIdList_.clear();
}

void DatabaseModel::GetIdList(QVariantList& _idList) const {
	_idList = selectedIdList_;
}

void DatabaseModel::searchPeriod(const QDate& _fromDate, const QDate& _toDate){
	qDebug() << "Try to find Preriod from:" << _fromDate.toString("yyyy-MM-dd") << "\tto:"<< _toDate.toString("yyyy-MM-dd");
	if(_fromDate > _toDate){
		qDebug() << "incorrect date range";
		return;
	}
	QSqlQuery query;
	if(query.prepare("SELECT id, " _DATE ", "
							_START_TIME ", " _END_TIME ", "
							_WORK_TIME ", " _WRITING_CODE_TIME ", "
							_WORK_SECS ", " _WRITING_CODE_SECS ", "
							_COMMENTS
							" FROM " TABLE_NAME
							" WHERE " _DATE " >= :dateFrom AND " _DATE " <= :dateTo")){
		query.bindValue(":dateFrom", _fromDate);
		query.bindValue(":dateTo", _toDate);
	} else {
		qDebug() << "SQL Prepare failed: search period";
		return;
	}
	if(!query.exec()){
		qDebug() << "SQL Error search period!";
		return;
	}
	this->setQuery(query);
}

void DatabaseModel::searhNote(QString _note, bool _similarBeginning){
	qDebug() << "Try to find note: " << _note;
	QString equal("=");
	if(_similarBeginning) {
		_note.append('%');
		equal = "LIKE";
	}
	QString queryStr  = QString("SELECT id, " _DATE ", "
								_START_TIME ", " _END_TIME ", "
								_WORK_TIME ", " _WRITING_CODE_TIME ", "
								_WORK_SECS ", " _WRITING_CODE_SECS ", "
								_COMMENTS
								" FROM " TABLE_NAME
								" WHERE " _COMMENTS " %1 :note;").arg(equal);
	QSqlQuery query;
	if(query.prepare(queryStr)){
		query.bindValue(":note", _note);
	} else {
		qDebug() << "SQL Prepare failed: search note";
		return;
	}
	if(!query.exec()){
		qDebug() << "SQL Error search note!";
		return;
	}
	this->setQuery(query);
}

int DatabaseModel::getIdByRowId(int _row) {
	return this->Data(this->index(_row, 0), IdRole).toInt();
}

QHash<int, QByteArray> DatabaseModel::RoleNames() const {
	QHash<int, QByteArray> roles;
	roles[IdRole] = "id";
	roles[DateRole] = "date";
	roles[StartTimeRole] = "startSession";
	roles[EndTimeRole] = "endSession";
	roles[WorkTimeRole] = "workTime";
	roles[WritingTimeRole] = "writingCodeTime";
	roles[WorkSecsRole] = "workSecs";
	roles[WritingCodeSecsRole] = "writingCodeSecs";
	roles[CommentsRole] = "comments";
	return roles;
}

}//namespace NS_Timer
