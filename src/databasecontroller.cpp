#include "databasecontroller.h"
#include "database.h"
#include "databasemodel.h"
#include "controller.h"

namespace NS_Timer
{

DataBaseController::DataBaseController(QObject* _parent, Controller* _controller)
: QObject(_parent)
, controller_(_controller)
{
	dataBase_	= std::make_shared<DataBase>(new DataBase(this, this));
	dataModel_	= std::make_shared<DataBaseModel>(new DataBaseModel(this));
}

void DataBaseController::ConnectToDB() {
	dataBase_->connectToDataBase();
}

void DataBaseController::CloseDB() {
	if(dataBase_ != nullptr){
		dataBase_->closeDataBase();
	}
}

bool DataBaseController::InsertIntoTable(const TimerData& _data) {
	if(dataBase_ != nullptr){
		return dataBase_->InsertIntoTable(_data);
	}
	return false;
}

void DataBaseController::ClearSelectedIdList() const noexcept {
	if(dataModel_ != nullptr){
		dataModel_->clearSelectedIdList();
	}
}

void DataBaseController::GetSelectedIdList(QVariantList& _idList) const {
	if(dataModel_ != nullptr){
		dataModel_->getSelectedIdList(_idList);
	}
}

bool DataBaseController::SearchDate(const QDate& _date) {
	if(dataModel_ != nullptr){
		return dataModel_->searchDate(_date);
	}
	return false;
}

bool DataBaseController::SearchDateUsingIdList(const QDate& _date) {
	if(dataModel_ != nullptr){
		return dataModel_->searchDateUsingIdList(_date);
	}
	return false;
}

void DataBaseController::GetPath(QString& _path) const noexcept{
	if(controller_ != nullptr){
		controller_->GetDirectoryPath(_path);
	}
}

DataBase& DataBaseController::DB() const {
	return (DataBase&)*dataBase_.data();
}

DataBaseModel& DataBaseController::DataModel() const {
	return (DataBaseModel&)*dataModel_.data();
}

QSqlQuery DataBaseController::CurrentQuery() const {
	if(dataModel_ != nullptr) {
		return dataModel_->getCurrentQueryModel();
	}
	return QSqlQuery();
}

void DataBaseController::UpdateModel() {
	if(dataModel_ != nullptr){
		dataModel_->updateModel();
	}
}

}//namespace NS_Timer
