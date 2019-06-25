#include "include/controller.h"
#include "include/symbolssettings.h"
#include "include/timer.h"
#include "include/keyeventfilter.h"
#include "include/trayiconwidget.h"
#include "include/database/databasecontroller.h"
#include "include/log.h"

#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>

Controller::Controller(QObject *parent) :
    QObject(parent)
{ }

Controller::~Controller() {
    qDebug() << "shutdown application";
}

void Controller::init() {
    _appPath = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/CodeTimer/");
    QDir dir;
    if(!dir.exists(_appPath))
        dir.mkpath(_appPath);
    try {
        _log = QSharedPointer<Log>(new Log(this, this));
        _log->initLogFile(_appPath);

        _symbolsSettings = QSharedPointer<SymbolsSettings>(new SymbolsSettings(this, this));
        _eventFilter = QSharedPointer<KeyEventFilter>(new KeyEventFilter());
        _dataBaseController = QSharedPointer<DataBaseController>(new DataBaseController(this, this));
        _timer = QSharedPointer<Timer>(new Timer(this, this));
        _trayIcon = QSharedPointer<TrayIconWidget>(new TrayIconWidget(this, this));
    } catch (std::exception ex) {
        qWarning() << ex.what();
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText(ex.what());
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
    // init key event filter
    _eventFilter->setController(this);
    _symbolsSettings->symbolListChanged(); // load to KeyEventFiler symbols for tracking

    // init database
    _dataBaseController->connectToDataBase();
    _dataBaseController->updateModel();
}

SymbolsSettings *Controller::getSymbolsSettings() const {
    return this->_symbolsSettings.data();
}

Timer *Controller::getTimer() const {
    return this->_timer.data();
}

TrayIconWidget *Controller::getSysTrayWidget() const {
    return this->_trayIcon.data();
}

DataBaseController *Controller::getDataBaseController() const {
    return _dataBaseController.data();
}

Log *Controller::getLog() const {
    return _log.data();
}

KeyEventFilter *Controller::getEventFilter() const {
    return _eventFilter.data();
}

void Controller::getSymbolsList(QList<unsigned long> &list) const {
    if(_symbolsSettings != nullptr){
        _symbolsSettings->getSymbolsList(list);
    }
}

void Controller::getDirectoryPath(QString &path) const noexcept{
    if(!_appPath.isEmpty()){
        path = _appPath;
    }
}

void Controller::sendSetSymbolListIntoFilter(const QList<unsigned long> &list) const{
    if(_eventFilter != nullptr){
        _eventFilter->setSymbolList(list);
    }
}

void Controller::sendControlKeyNoticed() {
    if(_timer != nullptr
            && _trayIcon != nullptr){
        _timer->setTimePoint();
        _trayIcon->setTrayIcon("start_noticed");
        QTimer::singleShot(200, this, [this]{
                                    _trayIcon->setTrayIcon("start_standart");
        });
    }
}

void Controller::sendRecordData(const TimerData *data) {
    if(_dataBaseController != nullptr){
        _dataBaseController->insertIntoTable(data);
    }
}

void Controller::sendSetTrayMenuActionEnable(const QString &actionName, bool isEnable) {
    if(_trayIcon != nullptr){
        _trayIcon->setActionEnable(actionName, isEnable);
    }
}

void Controller::sendSetTrayIcon(const QString &icon_name) {
    if(_trayIcon != nullptr){
        _trayIcon->setTrayIcon(icon_name);
    }
}

bool Controller::isTimerWorking() const noexcept{
    if(_timer != nullptr){
        return _timer->isTimerWorking();
    }
    return false;
}

void Controller::sendSetWindowsHook() const {
    if(_eventFilter != nullptr)
        _eventFilter->setWindowsHook();
}

void Controller::sendUnhookWindowsHook() const {
    if(_eventFilter != nullptr)
        _eventFilter->unhookWindowsHook();
}

void Controller::sendStartTimer(){
    if(_timer != nullptr){
        _timer->start();
    }
}

void Controller::sendPauseTimer() {
    if(_timer != nullptr){
        _timer->pause();
    }
}

void Controller::sendStopTimer() {
    if(_timer != nullptr){
        _timer->stop();
    }
}
