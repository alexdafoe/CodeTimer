#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include <QSqlQuery>

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
    // Constructor
    DataBaseController(QObject *parent, Controller* controller);
    // Destructor by default
    virtual ~DataBaseController() = default;

    // Sends connect to database
    void connectToDataBase();
    void closeDataBase();

    // Sends insert TimerData into table in database
    bool insertIntoTable(const TimerData *data);

    // Sends set clear list of id's which selected for unite in 1 row
    void clearSelectedIdList() const noexcept;
    // Sends select in database a date
    bool searchDate(const QDate &date);
    // Sends select in database a date if chosed couple row for unite
    bool searchDateUsingIdList(const QDate &date);

    // Returns a list of id's which selected for unite in 1 row
    void getSelectedIdList(QVariantList &idList) const;
    // Returns path for database and log files
    void getDirectoryPath(QString &path) const noexcept;

    // Returns a pointer to database
    DataBase* getDataBase() const;
    // Returns a pointer to database model view
    DataBaseModel *getDataModel() const;

    // Returns current query to database
    QSqlQuery getCurrentQueryModel() const;

public slots:
    void updateModel();

private:
    QSharedPointer<DataBase> _dataBase;
    QSharedPointer<DataBaseModel> _dataModel;
    Controller* _controller;
};

#endif // DATABASECONTROLLER_H
