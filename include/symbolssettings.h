#ifndef SYMBOLSSETTINGS_H
#define SYMBOLSSETTINGS_H

#include <QObject>

class Controller;

// This class is a control which symbols are tracking
class SymbolsSettings : public QObject
{
    Q_OBJECT
public:
    // Constructor
    SymbolsSettings(QObject *parent, Controller *controller);
    // Destructor
    virtual ~SymbolsSettings() = default;

    // Returns a list of symbols that tracking
    void getSymbolsList(QList<unsigned long> &list) const;
    // Event for settings tracking symbols to KeyEventFilter after someone state are changed
    void symbolListChanged() const;

    // State for tracking symbols "[ ] { }". Propagated to QML
    Q_PROPERTY(bool curlyBracket READ getCurlyBracket WRITE setCurlyBracket NOTIFY curlyBracketChanged) // { }
    bool getCurlyBracket() const noexcept;
    void setCurlyBracket(bool state);

    // State for tracking symbols ". , < >". Propagated to QML
    Q_PROPERTY(bool angleBracket READ getAngleBracket WRITE setAngleBracket NOTIFY angleBracketChanged) // < >
    bool getAngleBracket() const noexcept;
    void setAngleBracket(bool state);

    // State for tracking symbols "9 0 ( )". Propagated to QML
    Q_PROPERTY(bool parentheses READ getParentheses WRITE setParentheses NOTIFY parenthesesChanged) // ( )
    bool getParentheses() const noexcept;
    void setParentheses(bool state);

    // State for tracking symbols "; :". Propagated to QML
    Q_PROPERTY(bool semicolon READ getSemicolon WRITE setSemicolon NOTIFY semicolonChanged) // ;
    bool getSemicolon() const noexcept;
    void setSemicolon(bool state);

    // State for tracking symbols "*" (numpad) and "8 *" (main keyboard). Propagated to QML
    Q_PROPERTY(bool asterisk READ getAsterisk WRITE setAsterisk NOTIFY asteriskChanged) // *
    bool getAsterisk() const noexcept;
    void setAsterisk(bool state);

    // State for tracking symbols "3 #". Propagated to QML
    Q_PROPERTY(bool octothorpe READ getOctothorpe WRITE setOctothorpe NOTIFY octothorpeChanged) // #
    bool getOctothorpe() const noexcept;
    void setOctothorpe(bool state);

signals:
    void curlyBracketChanged(bool);
    void angleBracketChanged(bool);
    void parenthesesChanged(bool);
    void semicolonChanged(bool);
    void asteriskChanged(bool);
    void octothorpeChanged(bool);

protected:
    // Additional methods for add/erase key symbols to/from list
    void setCurlyBracketList(bool state);
    void setAngleBracketList(bool state);
    void setParenthesesList(bool state);
    void setSemicolonList(bool state);
    void setAsteriskList(bool state);
    void setOctothorpeList(bool state);

private:
    Controller *_controller;
    bool curlyBracket{ true };
    bool angleBracket{ true };
    bool parentheses{ true };
    bool semicolon{ true };
    bool asterisk{ true };
    bool octothorpe{ true };
    QList<unsigned long> _symbolList;
};

#endif // SYMBOLSSETTINGS_H
