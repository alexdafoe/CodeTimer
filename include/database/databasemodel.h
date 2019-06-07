#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>

class DataBaseModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1, // id
        DateRole, // date
        StartTimeRole, // start working
        EndTimeRole, // end working
        WorkTimeRole, // work time
        WritingTimeRole, // writing_code_time
        WorkSecsRole, // work seconds
        WritingCodeSecsRole, // writing code seconds
        CommentsRole // comments
    };
    explicit DataBaseModel(QObject *parent = nullptr);

    Q_PROPERTY(bool dateHasNote READ isDateHasNote WRITE setDateHasNote NOTIFY dateHasNoteChanged)
    bool isDateHasNote() const;
    void setDateHasNote(bool state);

    Q_PROPERTY(int dateRowsCount READ getDateRowsCount WRITE setDateRowsCount NOTIFY dateRowsCountChanged)
    int getDateRowsCount() const;
    void setDateRowsCount(int count);

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

    QSqlQuery getCurrentQueryModel() const;
    bool searchDate(const QDate &date);
    bool searchDateUsingIdList(const QDate &date);

    Q_INVOKABLE void addSelectedIdToList(int id);
    Q_INVOKABLE void clearSelectedIdList();
    void getSelectedIdList(QVariantList &idList) const;

public slots:
    void updateModel();
    void updateModelWithLastQuery();

    QVariantList getDateNotesList(const QDate &date);
    void searchPeriod(const QDate &dateFrom, const QDate &dateTo);
    void searhNote(QString note, bool similarBeginning);
    int getID(int row);

signals:
    void dateHasNoteChanged(bool);
    void dateRowsCountChanged(bool);
    void dateUniteIdListChanged();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    bool _dateHasNote{false};
    int _dateRowsCount{0};
    QVariantList _selectedIdList;
};

#endif // DATABASEMODEL_H
