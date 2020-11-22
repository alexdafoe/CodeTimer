#pragma once
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>

namespace NS_Timer
{

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
	explicit DataBaseModel(QObject* parent = nullptr);

	// Is the date has any note in different rows. Propagated to QML, for CombineDate layer
	Q_PROPERTY(bool			dateHasNote
						READ		IsDateHasNote
						WRITE	DateHasNote
						NOTIFY	DateHasNoteChanged)
	bool						IsDateHasNote()													const;
	void						DateHasNote(bool state);
	// The number of records by date. Propagated to QML, for CombineDate layer
	Q_PROPERTY(int			dateRowsCount
						READ		DateRowsCount
						WRITE	DateRowsCount
						NOTIFY	DateRowsCountChanged)
	int							DateRowsCount()												const;
	void						DateRowsCount(int count);

	// Display model view by role
	QVariant					Data(const QModelIndex&	item,
										int							role	= Qt::DisplayRole)	const;

	// Returns query model at the moment
	QSqlQuery				CurrentQuery()										const;
	// Query select in table by date
	bool						SearchDate(const QDate&);
	// Query select in table by date if chosed couple row for unite date
	bool						SearchDateWithIdList(const QDate&);

	// The list of id's which selected for unite in 1 row. Propagated to QML
	Q_INVOKABLE void	AddSelectedId(int id);
	Q_INVOKABLE void	ClearIdList() noexcept;
	void						GetIdList(QVariantList &idList)						const;

public slots:
	//  Propagated to QML

	void						UpdateModel();
	void						UpdateModelWithLastQuery();
	QVariantList			NoteByDate(const QDate&);
	void						SearchPeriod(const QDate& from, const QDate& to);
	void						SearhNote(QString note, bool similarBeginning);
	int							GetIdByRowId(int rowId);

signals:
	void						DateHasNoteChanged(bool);
	void						DateRowsCountChanged(bool);
	void						DateUnitedIdListChanged();

protected:
	// Returns hash map for model view
	QHash<int, QByteArray>	RoleNames()												const;

private:
	bool				dateHasNote_ = false;
	int					dateRowsCount_ = 0;
	QVariantList	selectedIdList_; // id list which selected by user
};

}//namespace NS_Timer
