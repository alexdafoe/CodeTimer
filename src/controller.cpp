#include "controller.h"
#include "log.h"
#include "symbolssettings.h"
#include "keyeventfilter.h"
#include "databasecontroller.h"
#include "timer.h"
#include "trayiconwidget.h"

#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>

namespace NS_Timer
{

Controller::Controller(QObject* _parent)
: QObject(_parent)
{}

Controller::~Controller() {
	qDebug() << "shutdown application";
}

void Controller::Init() {
	appPath_ = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/CodeTimer/");
	QDir dir;
	if(!dir.exists(appPath_))
		dir.mkpath(appPath_);
	try {
		logContext_					= std::make_shared<NS_Timer::LogContext>(new NS_Timer::LogContext(this, this));
		logContext_->Init(appPath_);

		symbolsSettings_			= std::make_shared<NS_Timer::SymbolsSettings>(new NS_Timer::SymbolsSettings(this, this));
		eventFilter_					= std::make_shared<KeyEventFilter>(new KeyEventFilter(this));
		dbController_				= std::make_shared<DataBaseController>(new DataBaseController(this, this));
		timer_						= std::make_shared<NS_Timer::Timer>(new NS_Timer::Timer(this, this));
		trayIcon_					= std::make_shared<TrayIconWidget>(new TrayIconWidget(this, this));
	} catch (std::exception ex) {
		qWarning() << ex.what();
		QMessageBox msg;
		msg.setIcon(QMessageBox::Critical);
		msg.setText(ex.what());
		msg.setStandardButtons(QMessageBox::Ok);
		msg.exec();
	}
	// init key event filter
	symbolsSettings_->SymbolListChanged(); // load to KeyEventFiler symbols for tracking

	// init database
	dbController_->ConnectToDB();
	dbController_->UpdateModel();
}

LogContext& Controller::LogContext() const
{
	return (NS_Timer::LogContext&)*logContext_;
}

SymbolsSettings& Controller::SymbolsSettings() const
{
	return (NS_Timer::SymbolsSettings&)*symbolsSettings_;
}

KeyEventFilter& Controller::EventFilter() const
{
	return (KeyEventFilter&)*eventFilter_;
}

DataBaseController& Controller::DatabaseController() const
{
	return (DataBaseController&)*dbController_;
}

Timer& Controller::Timer() const
{
	return (NS_Timer::Timer&)*timer_;
}

TrayIconWidget& Controller::SysTrayWidget() const
{
	return (TrayIconWidget&)*trayIcon_;
}

void Controller::GetTrackingSymbolsList(QList<unsigned long>& _list) const {
	if(symbolsSettings_ != nullptr){
		symbolsSettings_->GetTrackingSymbolsList(_list);
	}
}

void Controller::GetDirectoryPath(QString& _path) const noexcept{
	if(!appPath_.isEmpty()){
		_path = appPath_;
	}
}

void Controller::SendSetSymbolListIntoFilter(const QList<unsigned long>& _list) const{
	if(eventFilter_ != nullptr){
		eventFilter_->SetTrackingSymbolList(_list);
	}
}

void Controller::SendControlKeyDetected() {
	if(timer_ != nullptr
			&& trayIcon_ != nullptr){
		timer_->SetTimePoint();
		trayIcon_->SetTrayIcon("start_noticed");
		QTimer::singleShot(200, this, [this]{
									trayIcon_->SetTrayIcon("start_standart");
		});
	}
}

void Controller::SendRecordData(const TimerData& _data) {
	if(dbController_ != nullptr){
		dbController_->InsertIntoTable(_data);
	}
}

void Controller::SendSetTrayMenuActionEnable(const QString& _actionName, bool _isEnable) {
	if(trayIcon_ != nullptr){
		trayIcon_->SetActionEnable(_actionName, _isEnable);
	}
}

void Controller::SendSetTrayIcon(const QString& _iconName) {
	if(trayIcon_ != nullptr){
		trayIcon_->SetTrayIcon(_iconName);
	}
}

bool Controller::IsTimerWorking() const noexcept{
	if(timer_ != nullptr){
		return timer_->IsTimerWorking();
	}
	return false;
}

void Controller::SendSetWindowsHook() const {
	if(eventFilter_ != nullptr)
		eventFilter_->SetWindowsHook();
}

void Controller::SendUnhookWindowsHook() const {
	if(eventFilter_ != nullptr)
		eventFilter_->UnhookWindowsHook();
}

void Controller::SendStartTimer(){
	if(timer_ != nullptr){
		timer_->Start();
	}
}

void Controller::SendPauseTimer() {
	if(timer_ != nullptr){
		timer_->Pause();
	}
}

void Controller::SsendStopTimer() {
	if(timer_ != nullptr){
		timer_->Stop();
	}
}

}//namespace NS_Timer
