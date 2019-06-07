#include "include/database/databasemodel.h"
#include "include/database/database.h"
#include <QDate>
#include <QDebug>

DataBaseModel::DataBaseModel(QObject *parent) :
    QSqlQueryModel(parent)
{   }

bool DataBaseModel::isDateHasNote() const {
    return _dateHasNote;
}

void DataBaseModel::setDateHasNote(bool state) {
    if(_dateHasNote != state){
        _dateHasNote = state;
        emit dateHasNoteChanged(state);
    }
}

int DataBaseModel::getDateRowsCount() const {
    return _dateRowsCount;
}

void DataBaseModel::setDateRowsCount(int count) {
    if(count >= 0){
        _dateRowsCount = count;
        emit dateRowsCountChanged(count);
    }
}

QVariant DataBaseModel::data(const QModelIndex &item, int role) const {
    int columnId = role - Qt::UserRole -1;
    QModelIndex modelIndex = this->index(item.row(), columnId);
    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QSqlQuery DataBaseModel::getCurrentQueryModel() const {
    return this->query();
}

void DataBaseModel::updateModel() {
    QSqlQuery query;
    if(query.exec("SELECT * FROM " TABLE_NAME ";")){
        this->setQuery(query);
    } else {
        qDebug() << "SQL Error: update model";
    }
}

void DataBaseModel::updateModelWithLastQuery() {
    QSqlQuery query = this->query();
    if(query.exec()){
        this->setQuery(query);
    } else {
        qDebug() << "SQL Error: update model";
    }
}

QVariantList DataBaseModel::getDateNotesList(const QDate &date) {
    QVariantList list;
    list.clear();
    int rowsCount = 0;
    searchDate(date);
    if(this->query().exec()){
        QString str;
        while(this->query().next()){
            ++rowsCount;
            str = this->query().value(_COMMENTS).toString();
            if(!str.isEmpty() && !list.contains(str)){
                list.append(str);
            }
        }
    }
    setDateHasNote(!list.isEmpty());
    setDateRowsCount(rowsCount);
    return list;
}

bool DataBaseModel::searchDate(const QDate &date) {
    qDebug() << "Try to find Date:" << date.toString("yyyy-MM-dd");
    QSqlQuery query;
    if(query.prepare("SELECT id, " _DATE ", "
                  _START_TIME ", " _END_TIME ", "
                  _WORK_TIME ", " _WRITING_CODE_TIME ", "
                  _WORK_SECS ", " _WRITING_CODE_SECS ", "
                  _COMMENTS
                  " FROM " TABLE_NAME
                  " WHERE " _DATE " = :searchDate;")){
        query.bindValue(":searchDate", date);
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

bool DataBaseModel::searchDateUsingIdList(const QDate &date) {
    int listSize = _selectedIdList.count();
    if(listSize == 0)
        return searchDate(date);
    QString queryStr = QString("SELECT id, " _DATE ", "
                               _START_TIME ", " _END_TIME ", "
                               _WORK_TIME ", " _WRITING_CODE_TIME ", "
                               _WORK_SECS ", " _WRITING_CODE_SECS ", "
                               _COMMENTS
                               " FROM " TABLE_NAME
                               " WHERE id IN (");
    QStringList keys;
    keys.append(":sId");
    queryStr.append(":sId"); // 1st key w/o comma ','

    for(int i = 1; i < listSize; ++i){
        keys.append(QString(":sId%1").arg(i));
        queryStr.append(QString(", :sId%1").arg(i));
    }
    queryStr.append(");");
    QSqlQuery query;
    if(query.prepare(queryStr)){
        for(int j = 0; j < keys.count(); ++j)
            query.bindValue(keys.at(j), _selectedIdList.at(j).toInt());
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

void DataBaseModel::addSelectedIdToList(int id) {
    if(!_selectedIdList.contains(id))
        _selectedIdList.append(id);
}

void DataBaseModel::clearSelectedIdList() {
    _selectedIdList.clear();
}

void DataBaseModel::getSelectedIdList(QVariantList &idList) const {
    idList = _selectedIdList;
}

void DataBaseModel::searchPeriod(const QDate &dateFrom, const QDate &dateTo){
    qDebug() << "Try to find Preriod from:" << dateFrom.toString("yyyy-MM-dd") << "\tto:"<< dateTo.toString("yyyy-MM-dd");
    if(dateFrom > dateTo){
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
        query.bindValue(":dateFrom", dateFrom);
        query.bindValue(":dateTo", dateTo);
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

void DataBaseModel::searhNote(QString note, bool similarBeginning){
    qDebug() << "Try to find note: " << note;
    QString equal("=");
    if(similarBeginning) {
        note.append('%');
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
        query.bindValue(":note", note);
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

int DataBaseModel::getID(int row) {
    return this->data(this->index(row, 0), IdRole).toInt();
}

QHash<int, QByteArray> DataBaseModel::roleNames() const {
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
