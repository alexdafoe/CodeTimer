#include "include/database/database.h"
#include "include/database/databasecontroller.h"
#include "include/timerdata.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QFile>
#include <QDate>
#include <QDebug>


DataBase::DataBase(QObject *parent, DataBaseController *dbController):
    QObject(parent),
    _dbController(dbController)
{ }

DataBase::~DataBase() {
    this->closeDataBase();
}

void DataBase::connectToDataBase() {
    QFile dbFile;
    QString path;
    _dbController->getDirectoryPath(path);
    dbFile.setFileName(QString(path + DATABASE_NAME));
    if(!dbFile.exists()){
        this->restroreDataBase();
    } else {
        this->openDataBase();
    }
}

void DataBase::closeDataBase() {
    if(db.isOpen()){
        qDebug() << "close Database";
        db.close();
    }
}

QString DataBase::getNote() const {
    return _note;
}

void DataBase::setNote(const QString &note) {
    if(_note != note){
        _note = note;
        emit noteChanged(note);
    }
}

bool DataBase::insertIntoTable(const TimerData *data) {
    qDebug() << "check time for record data";
    if(data == nullptr)
        return false;
    if(data->getSecondsWritingCode() <= 0)
        return false;

    QSqlQuery query(db);
    if(query.prepare("INSERT INTO " TABLE_NAME " ( " _DATE ", "
                     _START_TIME ", " _END_TIME ", "
                     _WORK_TIME ", " _WRITING_CODE_TIME ", "
                     _WORK_SECS ", " _WRITING_CODE_SECS ", "
                     _COMMENTS ") "
                     "VALUES (:day, :start, :end, :worktime, :codetime, :worksecs, :codesecs, :note);"
                     )){
        query.bindValue(":day", data->getDateStart());
        query.bindValue(":start", data->getTimeStart());
        query.bindValue(":end", data->getTimeEnd());
        query.bindValue(":worktime", data->getTimeWork());
        query.bindValue(":codetime", data->getTimeWritingCode());
        query.bindValue(":worksecs", data->getSecondsWork());
        query.bindValue(":codesecs", data->getSecondsWritingCode());
        query.bindValue(":note", _note);
    } else {
        qDebug() << "SQL Prepare failed: insert TimerData into table";
        return false;
    }
    if(!query.exec()){
        qDebug() << "Error insert into " << TABLE_NAME << query.lastError().text();
        return false;
    }
    qDebug() << "Successfully recorded statistic data";
    _dbController->updateModel();
    return true;
}

bool DataBase::editNote(const int id, const QString &newNote) {
    qDebug() << "edit record id: " << id << " with new note: " << newNote;
    QSqlQuery query(db);
    if(query.prepare("UPDATE " TABLE_NAME " SET " _COMMENTS " = :newNote WHERE id = :ID ;")){
        query.bindValue(":newNote", newNote);
        query.bindValue(":ID", id);
    } else {
        qDebug() << "SQL Prepare edit note failed!";
        return false;
    }
    if(!query.exec()){
        qDebug() << "Error edit note with id:" << id << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::uniteDateWithNote(const QDate &date, const QString &note) {
    bool sqlState = true;
    QVector<int> idForDelete;
    QVariantList listOfKeys;
    int idUpdate = this->getIdUpdateDate(date, listOfKeys, idForDelete);
    if(idUpdate < 0)
        sqlState = false;

    db.transaction();
    while(sqlState){ // instead of enclosed "if"
        QSqlQuery queryUpd(db);
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
        queryUpd.bindValue(":note", note);
        queryUpd.bindValue(":ID", idUpdate);

        sqlState = queryUpd.exec();
        if(!sqlState){
            qDebug() << "Database error: can't Unite date" << queryUpd.lastError().text();
            break;
        }        
        break;
    }

    if(sqlState){
        sqlState = db.commit();
    }
    if(!sqlState){
        qDebug() << "ERROR transaction: Unite date" << date.toString("yy.MM.dd") << " with note" << note;
        db.rollback();
    } else {
        qDebug() << "Success transaction: Unite date" << date.toString("yy.MM.dd") << " with note" << note;
        _dbController->clearSelectedIdList();
        if(!idForDelete.isEmpty()){
            for(auto id : idForDelete)
                removeRecordById(id);
        }
    }
    return sqlState;
}

bool DataBase::removeRecordById(const int id) {
    QSqlQuery query(db);
    if(query.prepare("DELETE FROM " TABLE_NAME " WHERE id = :ID ;")){
        query.bindValue(":ID", id);
    } else {
        qDebug() << "SQL Prepare failed: delete record with id:" <<  id;
        return false;
    }

    if(!query.exec()){
        qDebug() << "Error delete row " << TABLE_NAME << query.lastError().text();
        return false;
    }
    qDebug() << "Successfully removed record with id: "<< id;
    return true;
}

bool DataBase::removeAllRecords() {
    QSqlQuery lastQuery = _dbController->getCurrentQueryModel();
    QString queryStr = lastQuery.executedQuery();
    if(queryStr.contains("WHERE")){
        return removeRecordsFromLastQuery(lastQuery);
    }
    QSqlQuery query(db);
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

bool DataBase::removeRecordsFromLastQuery(const QSqlQuery &lastQuery) {
    QStringList keys, values;
    QString newQueryStr = this->getStrQueryToBind(lastQuery, keys, values);
    newQueryStr.insert(0, "DELETE FROM " TABLE_NAME " WHERE ");

    QSqlQuery query(db);
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

QString DataBase::getNoteById(const int id) {
    QSqlQuery query(db);
    QString note;
    if(query.prepare("SELECT " _COMMENTS " FROM " TABLE_NAME " WHERE id = :ID ;")){
        query.bindValue(":ID", id);
    } else {
        qDebug() << "SQL Prepare failed: get note from id " << id;
        return note;
    }
    if(query.exec()){
        if(query.next()){
            note = query.value(_COMMENTS).toString();
        }
    }
    return note;
}

bool DataBase::openDataBase() {
    qDebug() << "open Database";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    QString path;
    _dbController->getDirectoryPath(path);
    db.setDatabaseName(path + DATABASE_NAME);
    return db.open();
}

bool DataBase::restroreDataBase() {
    if(this->openDataBase())
        return this->createTable();
    qWarning() << "Can't restore database";
    return false;
}

bool DataBase::createTable() {
    QSqlQuery query(db);
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

QString DataBase::getStrQueryToBind(const QSqlQuery &query, QStringList &keys, QStringList &values) {
    QString queryStr = query.executedQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());
    QStringList fields;
    while(it.hasNext()) { // parse query by keys and values
        it.next();
        keys.append(it.key());
        values.append(it.value().toString());
    }
    // cut out everything before 'where' stmt
    int pos = queryStr.indexOf("WHERE ");
    if(pos >= 0){
        queryStr.remove(0, pos + 6);
        queryStr.remove(QChar(';'), Qt::CaseInsensitive);
    }
    QStringList strListFields = queryStr.split(':'); // split by binded values (field = :key)
    strListFields.pop_back(); // erase last key

    for(int i = 1; i <= values.count(); ++i){ // fill fields list
        QString str = strListFields.at(i-1);
        if(i % 2 == 0) { // erase 'key'
            int p = str.indexOf(' ');
            str.remove(0, p);
        }
        fields.append(str);
    }
    QString newQueryStr;
    for(int i = 0; i < values.count(); ++i){
        newQueryStr.append(fields.at(i));
        newQueryStr.append(keys.at(i));
    }
    return newQueryStr;
}

int DataBase::getIdUpdateDate(const QDate &date, QVariantList &listOfKeys, QVector<int>& trashId) {
    QVariantList idList;
    _dbController->getSelectedIdList(idList);

    if(idList.isEmpty())
        _dbController->searchDate(date);
    else
        _dbController->searchDateUsingIdList(date);

    QSqlQuery query = _dbController->getCurrentQueryModel();
    QString startTmpStr, endTmpStr;
    QTime startSessionTime, startTmpTime;
    QTime endSessionTime, endTmpTime;
    int workSecs = 0;
    int writingCodeSecs = 0;
    int idUpd = -1;
    listOfKeys.clear();
    // Reserve for list 6 field:
    // 0 - startSession, 1 - endSession, 2 - workTime, 3 - codeTime, 4 -workSec, 5 - codeSec
    listOfKeys.reserve(6);
    if(query.exec()){
        if(query.next()){
            idUpd = query.value("id").toInt();
            listOfKeys.append(query.value(_START_TIME).toString()); // listOfKeys[0]
            startSessionTime = QTime::fromString(listOfKeys[0].toString(), "hh:mm:ss");
            endTmpStr = query.value(_END_TIME).toString();
            listOfKeys.append(endTmpStr); // listOfKeys[1]
            endTmpTime = QTime::fromString(endTmpStr, "hh:mm:ss");
            workSecs = query.value(_WORK_SECS).toInt();
            writingCodeSecs = query.value(_WRITING_CODE_SECS).toInt();
        }
        while(query.next()){
            trashId.append(query.value("id").toInt());
            startTmpStr = query.value(_START_TIME).toString();
            startTmpTime = QTime::fromString(startTmpStr, "hh:mm:ss");
            listOfKeys[1] = query.value(_END_TIME).toString();
            endSessionTime = QTime::fromString(listOfKeys[1].toString(), "hh:mm:ss");
            workSecs += query.value(_WORK_SECS).toInt();
            writingCodeSecs += query.value(_WRITING_CODE_SECS).toInt();
            if(startTmpTime < startSessionTime){ // make sure that start session time is correct
                listOfKeys[0] = startTmpStr;
            }
            if(endTmpTime > endSessionTime){ // make sure that end session time is correct
                listOfKeys[1] = endTmpStr;
            }
        }
        QString workTime, writingCodeTime;
        TimeSecToString()(workSecs, workTime);
        TimeSecToString()(writingCodeSecs, writingCodeTime);

        listOfKeys.append(workTime); // listOfKeys[2]
        listOfKeys.append(writingCodeTime); // listOfKeys[3]
        listOfKeys.append(workSecs); // listOfKeys[4]
        listOfKeys.append(writingCodeSecs); // listOfKeys[5]
    }
    return idUpd;
}
