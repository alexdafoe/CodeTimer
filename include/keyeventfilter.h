#ifndef KEYEVENTFILTER_H
#define KEYEVENTFILTER_H

#include <QTimer>
#include <Windows.h>
#include <WinUser.h>

class Controller;

class KeyEventFilter
{
public:
    KeyEventFilter();
    KeyEventFilter(const KeyEventFilter&) = delete;
    KeyEventFilter& operator=(const KeyEventFilter&) = delete;
    KeyEventFilter(KeyEventFilter&&) = delete;
    KeyEventFilter& operator=(KeyEventFilter&&) = delete;
    virtual ~KeyEventFilter();

    void setController(Controller *controller);
    void setSymbolList(const QList<unsigned long> &list);
    void setWindowsHook();
    void unhookWindowsHook();

    //WH_KEYBOARD hook procedure
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    static HHOOK keyHook;
    static QList<unsigned long> _symbolList;
    static Controller* _controller;
};

#endif // KEYEVENTFILTER_H
