#include "symbolssettings.h"
#include "keyeventfilter.h"
#include <QDebug>

namespace NS_Timer
{

SymbolsSettings::SymbolsSettings(std::shared_ptr<KeyEventFilter> _eventFilter)
: QObject(nullptr)
, eventFilter_(_eventFilter)
{
	// try to fill symbolList
	UpdateStateListCurlyBracket(true);
	UpdateStateListAngleBracket(true);
	UpdateStateListParentheses(true);
	UpdateStateListSemicolon(true);
	UpdateStateListAsterisk(true);
	UpdateStateListOctothorpe(true);
}

void SymbolsSettings::GetTrackingSymbolsList(QList<unsigned long>& _list) const{
	_list.clear();
	_list = symbolsList_;
}

void SymbolsSettings::SymbolListChanged() const {
	eventFilter_->SetTrackingSymbolList(symbolsList_);
}

bool SymbolsSettings::IsCurlyBracket() const noexcept {
	return curlyBracket_;
}

void SymbolsSettings::CurlyBracket(bool _state) {
	if(curlyBracket_ != _state){
		curlyBracket_ = _state;
		UpdateStateListCurlyBracket(_state);
		qDebug() << "CurlyBracket registration: " << _state;
		emit CurlyBracketStateChanged(_state);
	}
}

bool SymbolsSettings::IsAngleBracket() const noexcept {
	return angleBracket_;
}

void SymbolsSettings::AngleBracket(bool _state) {
	if(angleBracket_ != _state){
		angleBracket_ = _state;
		UpdateStateListAngleBracket(_state);
		qDebug() << "Angle registration: " << _state;
		emit AngleBracketStateChanged(_state);
	}
}

bool SymbolsSettings::IsParentheses() const noexcept {
	return parentheses_;
}

void SymbolsSettings::Parentheses(bool _state) {
	if(parentheses_ != _state){
		parentheses_ = _state;
		UpdateStateListParentheses(_state);
		qDebug() << "Parentheses registration: " << _state;
		emit ParenthesesStateChanged(_state);
	}
}

bool SymbolsSettings::IsSemicolon() const noexcept {
	return semicolon_;
}

void SymbolsSettings::Semicolon(bool _state) {
	if(semicolon_ != _state){
		semicolon_ = _state;
		UpdateStateListSemicolon(_state);
		qDebug() << "Semicolon registration: " << _state;
		emit SemicolonStateChanged(_state);
	}
}

bool SymbolsSettings::IsAsterisk() const noexcept {
	return asterisk_;
}

void SymbolsSettings::Asterisk(bool _state) {
	if(asterisk_ != _state){
		asterisk_ = _state;
		UpdateStateListAsterisk(_state);
		qDebug() << "Asterisk registration: " << _state;
		emit AsteriskStateChanged(_state);
	}
}

bool SymbolsSettings::IsOctothorpe() const noexcept {
	return octothorpe_;
}

void SymbolsSettings::Octothorpe(bool _state) {
	if(octothorpe_ != _state){
		octothorpe_ = _state;
		UpdateStateListOctothorpe(_state);
		qDebug() << "Octothorpe registration: " << _state;
		emit OctothorpeStateChanged(_state);
	}
}

//Protected methods
void SymbolsSettings::UpdateStateListCurlyBracket(bool _state) {
	if(_state){
		symbolsList_.append(VK_OEM_4); // [{
		symbolsList_.append(VK_OEM_6); // ]}
	} else {
		symbolsList_.removeOne(VK_OEM_4);
		symbolsList_.removeOne(VK_OEM_6);
	}
	SymbolListChanged();
}

void SymbolsSettings::UpdateStateListAngleBracket(bool _state) {
	if(_state){
		symbolsList_.append(VK_OEM_COMMA); // ,<
		symbolsList_.append(VK_OEM_PERIOD); // .>
	} else {
		symbolsList_.removeOne(VK_OEM_COMMA);
		symbolsList_.removeOne(VK_OEM_PERIOD);
	}
	SymbolListChanged();
}

void SymbolsSettings::UpdateStateListParentheses(bool _state) {
	if(_state){
		symbolsList_.append(0x39); // 9 (
		symbolsList_.append(0x30); // 0 )
	} else {
		symbolsList_.removeOne(0x39);
		symbolsList_.removeOne(0x30);
	}
	SymbolListChanged();
}

void SymbolsSettings::UpdateStateListSemicolon(bool _state) {
	if(_state){
		symbolsList_.append(VK_OEM_1); // ;:
	} else {
		symbolsList_.removeOne(VK_OEM_1);
	}
	SymbolListChanged();
}

void SymbolsSettings::UpdateStateListAsterisk(bool _state) {
	if(_state){
		symbolsList_.append(VK_MULTIPLY); // * (numpad)
		symbolsList_.append(0x38); // 8 * (main keyboard)
	} else {
		symbolsList_.removeOne(VK_MULTIPLY);
		symbolsList_.removeOne(0x38);
	}
	SymbolListChanged();
}

void SymbolsSettings::UpdateStateListOctothorpe(bool _state) {
	if(_state){
		symbolsList_.append(0x33); // 3 (#)
	} else {
		symbolsList_.removeOne(0x33);
	}
	SymbolListChanged();
}

}//namespace NS_Timer
