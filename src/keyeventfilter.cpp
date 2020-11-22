#include "keyeventfilter.h"
#include "controller.h"
#include <stdio.h>
#include <QDebug>

//Static variable
HHOOK KeyEventFilter::_keyHook = nullptr;
QList<unsigned long> KeyEventFilter::_symbolList = { };
Controller* KeyEventFilter::_controller = nullptr;

//Methods
KeyEventFilter::KeyEventFilter(Controller *controller) {
	_controller = controller;
}

KeyEventFilter::~KeyEventFilter(){
	unhookWindowsHook();
}

void KeyEventFilter::setTrackingSymbolList(const QList<unsigned long> &list) {
	_symbolList = list;
}

void KeyEventFilter::setWindowsHook(){
	HMODULE hInstance = GetModuleHandle(nullptr);
	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
	if(hook != nullptr){
		_keyHook = hook;
		qDebug() << "Key Hook: enabled";
	}
}

void KeyEventFilter::unhookWindowsHook() {
	if(UnhookWindowsHookEx(_keyHook))
		qDebug() << "Key Hook: disabled";
}

LRESULT KeyEventFilter::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if(nCode == HC_ACTION
			&& wParam == WM_KEYDOWN
			&& _controller->isTimerWorking()) {
		PKBDLLHOOKSTRUCT key = PKBDLLHOOKSTRUCT(lParam);
		if(_symbolList.contains(key->vkCode)){
			qDebug() << "special key detected:" << key->vkCode;
			_controller->sendControlKeyDetected();
		}
	}
	return CallNextHookEx(_keyHook, nCode, wParam, lParam);
}
