#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include <QSqlQuery>

class DataBase;
class DataBaseModel;
class Controller;
class TimerData;

class DataBaseController : public QObject
{
    Q_OBJECT
public:
    DataBaseController(QObject *parent, Controller* controller);
    virtual ~DataBaseController() = default;

    void connectToDataBase();
    void closeDataBase();

    bool insertIntoTable(const QString& table, const TimerData *data);

    void clearSelectedIdList() const;
    bool searchDate(const QDate &date);
    bool searchDateUsingIdList(const QDate &date);

    void getSelectedIdList(QVariantList &idList) const;
    void getDirectoryPath(QString &path);
    DataBase* getDataBase() const;
    DataBaseModel *getDataModel() const;
    QSqlQuery getCurrentQueryModel() const;

public slots:
    void updateModel();
    void removeAllRecords();

private:
    QSharedPointer<DataBase> _dataBase;
    QSharedPointer<DataBaseModel> _dataModel;
    Controller* _controller;
};

#endif // DATABASECONTROLLER_H
