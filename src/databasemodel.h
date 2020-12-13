#pragma once
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>

namespace NS_Timer
{

// This class is a database model view
// Provide view from SQLite database to QML
class DatabaseModel : public QSqlQueryModel
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

	explicit DatabaseModel(QObject* parent = nullptr);
	DatabaseModel(const DatabaseModel&)											= delete;
	DatabaseModel(DatabaseModel&&)													= delete;
	virtual ~DatabaseModel()																= default;

	DatabaseModel&		operator=(const DatabaseModel&)						= delete;
	DatabaseModel&		operator=(DatabaseModel&&)								= delete;

	// Is the date has any note in different rows. Propagated to QML, for CombineDate layer
	Q_PROPERTY(bool			IsDateHasNote
						READ		IsDateHasNote
						WRITE	DateHasNote
						NOTIFY	dateHasNoteChanged)
	bool						IsDateHasNote()													const noexcept { return dateHasNote_; }
	void						DateHasNote(bool state);
	// The number of records by date. Propagated to QML, for CombineDate layer
	Q_PROPERTY(int			DateRowsCount
						READ		DateRowsCount
						WRITE	DateRowsCount
						NOTIFY	dateRowsCountChanged)
	int							DateRowsCount()													const noexcept { return dateRowsCount_; }
	void						DateRowsCount(int count);

	// Display model view by role
	QVariant					Data(const QModelIndex&	item,
										int							role	= Qt::DisplayRole)	const;

	QSqlQuery				CurrentQuery()													const;
	// Query select in table by date
	bool						SearchDate(const QDate&);
	// Query select in table by date if chosed couple row for unite date
	bool						SearchDateWithIdList(const QDate&);

	// The list of id's which selected for unite in 1 row. Propagated to QML
	Q_INVOKABLE void	addSelectedId(int id);
	Q_INVOKABLE void	clearIdList() noexcept;
	void						GetIdList(QVariantList &idList)									const;

public slots:
	//  Propagated to QML
	void						update();
	void						updateWithLastQuery();
	QVariantList			noteByDate(const QDate&);
	void						searchPeriod(const QDate& from, const QDate& to);
	void						searhNote(QString note, bool similarBeginning);
	int							getIdByRowId(int rowId);

signals:
	void						dateHasNoteChanged(bool);
	void						dateRowsCountChanged(bool);

protected:
	// Returns hash map for model view
	QHash<int, QByteArray>	RoleNames()												const;

private:
	bool				dateHasNote_ = false;
	int					dateRowsCount_ = 0;
	QVariantList	selectedIdList_; // id list which selected by user
};

}//namespace NS_Timer
