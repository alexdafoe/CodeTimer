#include "include/symbolssettings.h"
#include "include/controller.h"
#include <windows.h>
#include <QDebug>

SymbolsSettings::SymbolsSettings(QObject *parent, Controller *controller):
    QObject(parent),
    _controller(controller)
{
    // try to fill symbolList
    setCurlyBracketList(curlyBracket);
    setAngleBracketList(angleBracket);
    setParenthesesList(parentheses);
    setSemicolonList(semicolon);
    setAsteriskList(asterisk);
    setOctothorpeList(octothorpe);
}

void SymbolsSettings::getSymbolsList(QList<unsigned long> &list) const{
    list.clear();
    list = _symbolList;
}

void SymbolsSettings::symbolListChanged() const {
    if(_controller != nullptr)
        _controller->sendSetSymbolListIntoFilter(_symbolList);
}

bool SymbolsSettings::getCurlyBracket() const noexcept {
    return curlyBracket;
}

void SymbolsSettings::setCurlyBracket(bool state) {
    if(curlyBracket != state){
        curlyBracket = state;
        setCurlyBracketList(state);
        qDebug() << "CurlyBracket registration: " << state;
        emit curlyBracketChanged(state);
    }
}

bool SymbolsSettings::getAngleBracket() const noexcept {
    return angleBracket;
}

void SymbolsSettings::setAngleBracket(bool state) {
    if(angleBracket != state){
        angleBracket = state;
        setAngleBracketList(state);
        qDebug() << "Angle registration: " << state;
        emit angleBracketChanged(state);
    }
}

bool SymbolsSettings::getParentheses() const noexcept {
    return parentheses;
}

void SymbolsSettings::setParentheses(bool state) {
    if(parentheses != state){
        parentheses = state;
        setParenthesesList(state);
        qDebug() << "Parentheses registration: " << state;
        emit parenthesesChanged(state);
    }
}

bool SymbolsSettings::getSemicolon() const noexcept {
    return semicolon;
}

void SymbolsSettings::setSemicolon(bool state) {
    if(semicolon != state){
        semicolon = state;
        setSemicolonList(state);
        qDebug() << "Semicolon registration: " << state;
        emit semicolonChanged(state);
    }
}

bool SymbolsSettings::getAsterisk() const noexcept {
    return asterisk;
}

void SymbolsSettings::setAsterisk(bool state) {
    if(asterisk != state){
        asterisk = state;
        setAsteriskList(state);
        qDebug() << "Asterisk registration: " << state;
        emit asteriskChanged(state);
    }
}

bool SymbolsSettings::getOctothorpe() const noexcept {
    return octothorpe;
}

void SymbolsSettings::setOctothorpe(bool state) {
    if(octothorpe != state){
        octothorpe = state;
        setOctothorpeList(state);
        qDebug() << "Octothorpe registration: " << state;
        emit octothorpeChanged(state);
    }
}

//Protected methods
void SymbolsSettings::setCurlyBracketList(bool state) {
    if(state){
        _symbolList.append(VK_OEM_4); // [{
        _symbolList.append(VK_OEM_6); // ]}
    } else {
        _symbolList.removeOne(VK_OEM_4);
        _symbolList.removeOne(VK_OEM_6);
    }
    symbolListChanged();
}

void SymbolsSettings::setAngleBracketList(bool state) {
    if(state){
        _symbolList.append(VK_OEM_COMMA); // ,<
        _symbolList.append(VK_OEM_PERIOD); // .>
    } else {
        _symbolList.removeOne(VK_OEM_COMMA);
        _symbolList.removeOne(VK_OEM_PERIOD);
    }
    symbolListChanged();
}

void SymbolsSettings::setParenthesesList(bool state) {
    if(state){
        _symbolList.append(0x39); // 9 (
        _symbolList.append(0x30); // 0 )
    } else {
        _symbolList.removeOne(0x39);
        _symbolList.removeOne(0x30);
    }
    symbolListChanged();
}

void SymbolsSettings::setSemicolonList(bool state) {
    if(state){
        _symbolList.append(VK_OEM_1); // ;:
    } else {
        _symbolList.removeOne(VK_OEM_1);
    }
    symbolListChanged();
}

void SymbolsSettings::setAsteriskList(bool state) {
    if(state){
        _symbolList.append(VK_MULTIPLY); // * (numpad)
        _symbolList.append(0x38); // 8 * (main keyboard)
    } else {
        _symbolList.removeOne(VK_MULTIPLY);
        _symbolList.removeOne(0x38);
    }
    symbolListChanged();
}

void SymbolsSettings::setOctothorpeList(bool state) {
    if(state){
        _symbolList.append(0x33); // 3 (#)
    } else {
        _symbolList.removeOne(0x33);
    }
    symbolListChanged();
}
