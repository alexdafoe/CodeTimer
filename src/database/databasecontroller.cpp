#include "include/database/databasecontroller.h"
#include "include/database/database.h"
#include "include/database/databasemodel.h"
#include "include/controller.h"

DataBaseController::DataBaseController(QObject *parent, Controller *controller):
    QObject(parent),
    _controller(controller)
{
    _dataBase = QSharedPointer<DataBase>(new DataBase(this, this));
    _dataModel = QSharedPointer<DataBaseModel>(new DataBaseModel(this));
}

void DataBaseController::connectToDataBase() {
    _dataBase->connectToDataBase();
}

void DataBaseController::closeDataBase() {
    if(_dataBase != nullptr){
        _dataBase->closeDataBase();
    }
}

bool DataBaseController::insertIntoTable(const QString &table, const TimerData *data) {
    Q_UNUSED(table);
    if(_dataBase != nullptr){
        return _dataBase->insertIntoTable(data);
    }
    return false;
}

void DataBaseController::clearSelectedIdList() const {
    if(_dataModel != nullptr){
        _dataModel->clearSelectedIdList();
    }
}

void DataBaseController::getSelectedIdList(QVariantList &idList) const {
    if(_dataModel != nullptr){
        _dataModel->getSelectedIdList(idList);
    }
}

bool DataBaseController::searchDate(const QDate &date) {
    if(_dataModel != nullptr){
        return _dataModel->searchDate(date);
    }
    return false;
}

bool DataBaseController::searchDateUsingIdList(const QDate &date) {
    if(_dataModel != nullptr){
        return _dataModel->searchDateUsingIdList(date);
    }
    return false;
}

void DataBaseController::getDirectoryPath(QString &path) {
    if(_controller != nullptr){
        _controller->getDirectoryPath(path);
    }
}

DataBase *DataBaseController::getDataBase() const {
    return _dataBase.data();
}

DataBaseModel *DataBaseController::getDataModel() const {
    return _dataModel.data();
}

QSqlQuery DataBaseController::getCurrentQueryModel() const {
    if(_dataModel != nullptr) {
        return _dataModel->getCurrentQueryModel();
    }
    return QSqlQuery();
}

void DataBaseController::updateModel() {
    if(_dataModel != nullptr){
        _dataModel->updateModel();
    }
}

void DataBaseController::removeAllRecords() {
    if(_dataBase != nullptr){
        _dataBase->removeAllRecords();
    }
}
