#include "keyeventfilter.h"
#include "controller.h"
#include <stdio.h>
#include <QDebug>

namespace NS_Timer
{

//Static variable
HHOOK KeyEventFilter::keyHook_ = nullptr;
QList<unsigned long> KeyEventFilter::symbolList_ = { };
Controller* KeyEventFilter::controller_ = nullptr;

//Methods
KeyEventFilter::KeyEventFilter(Controller* _controller) {
	controller_ = _controller;
}

KeyEventFilter::~KeyEventFilter(){
	UnhookWindowsHook();
}

void KeyEventFilter::SetTrackingSymbolList(const QList<unsigned long>& _newSymbolList) {
	symbolList_ = _newSymbolList;
}

void KeyEventFilter::SetWindowsHook(){
	HMODULE hInstance = GetModuleHandle(nullptr);
	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
	if(hook != nullptr){
		keyHook_ = hook;
		qDebug() << "Key Hook: enabled";
	}
}

void KeyEventFilter::UnhookWindowsHook() {
	if(UnhookWindowsHookEx(keyHook_))
		qDebug() << "Key Hook: disabled";
}

LRESULT KeyEventFilter::KeyboardProc(int _nCode, WPARAM _wParam, LPARAM _lParam) {
	if(_nCode == HC_ACTION
			&& _wParam == WM_KEYDOWN
			&& controller_->IsTimerWorking()) {
		PKBDLLHOOKSTRUCT key = PKBDLLHOOKSTRUCT(_lParam);
		if(symbolList_.contains(key->vkCode)){
			qDebug() << "special key detected:" << key->vkCode;
			controller_->SendControlKeyDetected();
		}
	}
	return CallNextHookEx(keyHook_, _nCode, _wParam, _lParam);
}

}//namespace NS_Timer
