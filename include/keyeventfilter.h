#ifndef KEYEVENTFILTER_H
#define KEYEVENTFILTER_H

#include <QList>
#include <Windows.h>
#include <WinUser.h>

class Controller;

// This class is event filter
// Tracking keyboard using Win API
class KeyEventFilter
{
public:
    // Constructor
    KeyEventFilter(Controller *controller);
    KeyEventFilter(const KeyEventFilter&) = delete;
    KeyEventFilter& operator=(const KeyEventFilter&) = delete;
    KeyEventFilter(KeyEventFilter&&) = delete;
    KeyEventFilter& operator=(KeyEventFilter&&) = delete;
    // Destructor
    virtual ~KeyEventFilter();

    // Set key symbols for tracking
    void setSymbolList(const QList<unsigned long> &list);
    // Enable windows hook for tracking keyboard
    void setWindowsHook();
    // Disable windows hook for tracking keyboard
    void unhookWindowsHook();

    //WH_KEYBOARD hook procedure
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    static HHOOK _keyHook;
    static QList<unsigned long> _symbolList;
    static Controller* _controller;
};

#endif // KEYEVENTFILTER_H
