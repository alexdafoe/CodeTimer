#pragma once
#include "controller.h"
#include <QList>
#include <Windows.h>
#include <WinUser.h>

namespace NS_Timer
{

// This class is event filter
// Tracking keyboard using Win API
class KeyEventFilter
{
public:
	explicit KeyEventFilter(Controller*);
	KeyEventFilter(const KeyEventFilter&)										= delete;
	KeyEventFilter(KeyEventFilter&&)											= delete;
	virtual ~KeyEventFilter();

	KeyEventFilter&					operator=(const KeyEventFilter&)	= delete;
	KeyEventFilter&					operator=(KeyEventFilter&&)			= delete;

	void									SetTrackingSymbolList(const QList<unsigned long>&);
	void									SetWindowsHook();
	void									UnhookWindowsHook();

	//WH_KEYBOARD hook procedure
	static LRESULT CALLBACK		KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
	static Controller*					controller_;
	static HHOOK						keyHook_;
	static QList<unsigned long>	symbolList_;
};

}//namespace NS_Timer
