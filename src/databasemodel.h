#pragma once
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>

// This class is a database model view
// Provide view from SQLite database to QML
class DataBaseModel : public QSqlQueryModel
{
	Q_OBJECT
public:
	// Column roles
	enum Roles {
		IdRole = Qt::UserRole + 1, // id
		DateRole, // date
		StartTimeRole, // start working
		EndTimeRole, // end working
		WorkTimeRole, // work time
		WritingTimeRole, // writing code time
		WorkSecsRole, // work seconds
		WritingCodeSecsRole, // writing code seconds
		CommentsRole // comments, note
	};
	// Constructor
	explicit DataBaseModel(QObject *parent = nullptr);

	// Is the date has any note in different rows. Propagated to QML, for CombineDate layer
	Q_PROPERTY(bool			dateHasNote
						READ		isDateHasNote
						WRITE	setDateHasNote
						NOTIFY	dateHasNoteChanged)
	bool						isDateHasNote()													const;
	void						setDateHasNote(bool state);
	// The number of records by date. Propagated to QML, for CombineDate layer
	Q_PROPERTY(int			dateRowsCount
						READ		getDateRowsCount
						WRITE	setDateRowsCount
						NOTIFY	dateRowsCountChanged)
	int							getDateRowsCount()												const;
	void						setDateRowsCount(int count);

	// Display model view by role
	QVariant					data(const QModelIndex&	item,
										int							role	= Qt::DisplayRole)	const;

	// Returns query model at the moment
	QSqlQuery				getCurrentQueryModel()										const;
	// Query select in table by date
	bool						searchDate(const QDate &date);
	// Query select in table by date if chosed couple row for unite date
	bool						searchDateUsingIdList(const QDate &date);

	// The list of id's which selected for unite in 1 row. Propagated to QML
	Q_INVOKABLE void	addSelectedIdToList(int id);
	Q_INVOKABLE void	clearSelectedIdList() noexcept;
	void						getSelectedIdList(QVariantList &idList)						const;

public slots:
	//  Propagated to QML

	void						updateModel();
	void						updateModelWithLastQuery();
	// Returns list of note's by date
	QVariantList			getDateNotesList(const QDate &date);
	// Query select in table span of date's
	void						searchPeriod(const QDate &dateFrom, const QDate &dateTo);
	// Query select in table by note
	void						searhNote(QString note, bool similarBeginning);
	// Returns id by clicking left button mouse in model view
	int							getID(int row);

signals:
	void						dateHasNoteChanged(bool);
	void						dateRowsCountChanged(bool);
	void						dateUniteIdListChanged();

protected:
	// Returns hash map for model view
	QHash<int, QByteArray>	roleNames()												const;

private:
	bool				_dateHasNote{false};
	int					_dateRowsCount{0};
	QVariantList	_selectedIdList; // id list which selected by user
};

