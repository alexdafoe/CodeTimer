#pragma once
#include <QList>
#include <Windows.h>
#include <WinUser.h>

class Controller;

// This class is event filter
// Tracking keyboard using Win API
class KeyEventFilter
{
public:
	KeyEventFilter(Controller *controller);
	KeyEventFilter(const KeyEventFilter&)										= delete;
	KeyEventFilter(KeyEventFilter&&)											= delete;
	virtual ~KeyEventFilter();

	KeyEventFilter&					operator=(const KeyEventFilter&)	= delete;
	KeyEventFilter&					operator=(KeyEventFilter&&)			= delete;

	void									setTrackingSymbolList(const QList<unsigned long> &list);
	void									setWindowsHook();
	void									unhookWindowsHook();

	//WH_KEYBOARD hook procedure
	static LRESULT CALLBACK		KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
	static HHOOK						_keyHook;
	static QList<unsigned long>	_symbolList;
	static Controller*					_controller;
};
