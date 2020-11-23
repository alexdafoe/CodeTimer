#pragma once
#include <QObject>
#include <memory>

namespace NS_Timer
{

class KeyEventFilter;

// This class is a control which symbols are tracking
class SymbolsSettings : public QObject
{
	Q_OBJECT
public:
	SymbolsSettings(std::shared_ptr<KeyEventFilter>);
	SymbolsSettings(const SymbolsSettings&)											= delete;
	SymbolsSettings(const SymbolsSettings&&)											= delete;
	virtual ~SymbolsSettings()																	= default;

	SymbolsSettings&		operator=(const SymbolsSettings&)							= delete;
	SymbolsSettings&		operator=(const SymbolsSettings&&)						= delete;

	//todo: init instead of UpdateList in Constructor
	void						GetTrackingSymbolsList(QList<unsigned long>& list)	const;
	// Event for settings tracking symbols to KeyEventFilter after someone state are changed
	void						SymbolListChanged() const;

	// States for tracking symbols. Propagated to QML
	Q_PROPERTY(bool			curlyBracket_
						READ		IsCurlyBracket
						WRITE	CurlyBracket
						NOTIFY	CurlyBracketStateChanged) // [ ] { }
	bool						IsCurlyBracket()													const noexcept;
	void						CurlyBracket(bool state);

	Q_PROPERTY(bool			angleBracket_
						READ		IsAngleBracket
						WRITE	AngleBracket
						NOTIFY	AngleBracketStateChanged) // < >
	bool						IsAngleBracket()													const noexcept;
	void						AngleBracket(bool state);

	Q_PROPERTY(bool			parentheses_
						READ		IsParentheses
						WRITE	IsParentheses
						NOTIFY	ParenthesesStateChanged) // 9 0 ( )
	bool						IsParentheses()													const noexcept;
	void						Parentheses(bool state);

	Q_PROPERTY(bool			semicolon_
						READ		IsSemicolon
						WRITE	Semicolon
						NOTIFY	SemicolonStateChanged) // ; :
	bool						IsSemicolon()														const noexcept;
	void						Semicolon(bool state);

	Q_PROPERTY(bool			asterisk_
						READ		IsAsterisk
						WRITE	Asterisk
						NOTIFY	AsteriskStateChanged) // "*" (numpad) and "8 *"
	bool						IsAsterisk()															const noexcept;
	void						Asterisk(bool state);

	Q_PROPERTY(bool			octothorpe_
						READ		IsOctothorpe
						WRITE	Octothorpe
						NOTIFY	OctothorpeStateChanged) // 3 #
	bool						IsOctothorpe()														const noexcept;
	void						Octothorpe(bool state);

signals:
	void						CurlyBracketStateChanged(bool);
	void						AngleBracketStateChanged(bool);
	void						ParenthesesStateChanged(bool);
	void						SemicolonStateChanged(bool);
	void						AsteriskStateChanged(bool);
	void						OctothorpeStateChanged(bool);

protected:
	// Additional methods for add/erase key symbols to/from list
	void						UpdateStateListCurlyBracket(bool state);
	void						UpdateStateListAngleBracket(bool state);
	void						UpdateStateListParentheses(bool state);
	void						UpdateStateListSemicolon(bool state);
	void						UpdateStateListAsterisk(bool state);
	void						UpdateStateListOctothorpe(bool state);

private:
	std::shared_ptr<KeyEventFilter>		eventFilter_;
	bool											curlyBracket_	= true;
	bool											angleBracket_	= true;
	bool											parentheses_	= true;
	bool											semicolon_		= true;
	bool											asterisk_			= true;
	bool											octothorpe_		= true;
	QList<unsigned long>					symbolsList_;
};

}//namespace NS_Timer
