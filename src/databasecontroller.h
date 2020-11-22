#pragma once
#include <QObject>
#include <QSqlQuery>
#include <memory>

namespace NS_Timer
{

class DataBase;
class DataBaseModel;
class Controller;
class TimerData;

// This class is divider between database classes and other classes
// Also provide access between query and model view
class DataBaseController : public QObject
{
	Q_OBJECT
public:
	DataBaseController(QObject* parent, Controller*);
	virtual ~DataBaseController() = default;

	void					ConnectToDB();
	void					CloseDB();

	bool					InsertIntoTable(const TimerData&);

	// Sends set clear list of id's which selected for unite in 1 row
	void					ClearSelectedIdList()								const noexcept;
	// Sends select in database a date
	bool					SearchDate(const QDate&);
	// Sends select in database a date if chosed couple row for unite
	bool					SearchDateUsingIdList(const QDate&);

	// Returns a list of id's which selected for unite in 1 row
	void					GetSelectedIdList(QVariantList& idList)		const;
	// Returns path for database and log files
	void					GetPath(QString& path)							const noexcept;

	DataBase&			DB()													const;
	DataBaseModel&	DataModel()										const;

	QSqlQuery			CurrentQuery()									const;

public slots:
	void					UpdateModel();

private:
	std::shared_ptr<DataBase>			dataBase_;
	std::shared_ptr<DataBaseModel>	dataModel_;
	Controller*									controller_;
};

}//namespace NS_Timer
