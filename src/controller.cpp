#include "controller.h"
#include "log.h"
#include "database.h"
#include "databasemodel.h"
#include "symbolssettings.h"
#include "keyeventfilter.h"
#include "timer.h"
#include "trayiconwidget.h"
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>

namespace NS_Timer
{

Controller::Controller()
: QObject(nullptr)
, appPath_(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/CodeTimer/")
{
	QDir dir;
	if(!dir.exists(appPath_))
		dir.mkpath(appPath_);

	logContext_			= std::make_shared<LogContext>(appPath_);
	dbModel_			= std::make_shared<DatabaseModel>(this);
	dbWrap_				= std::make_shared<DatabaseWrap>(dbModel_, appPath_);
	eventFilter_			= std::make_shared<KeyEventFilter>(this);
	symbolsSettings_	= std::make_shared<SymbolsSettings>(eventFilter_);
	timer_				= std::make_shared<Timer>(*this);
	trayIcon_			= std::make_shared<TrayIconWidget>(this);
}

Controller::~Controller() {
	qDebug() << "shutdown application";
	logContext_->Done();
}

void Controller::Init() {
	logContext_->Init();
	// init key event filter
	symbolsSettings_->SymbolListChanged(); // load to KeyEventFiler symbols for tracking

	// init database
	dbWrap_->ConnectToDB();
	dbModel_->update();
}

LogContext& Controller::Log() const
{
	return (LogContext&)*logContext_;
}

SymbolsSettings& Controller::KeyControlSettings() const
{
	return (SymbolsSettings&)*symbolsSettings_;
}

KeyEventFilter& Controller::EventFilter() const
{
	return (KeyEventFilter&)*eventFilter_;
}

DatabaseWrap& Controller::DB() const
{
	return (DatabaseWrap&)*dbWrap_;
}

DatabaseModel& Controller::DBModel() const
{
	return (DatabaseModel&)*dbModel_;
}

Timer& Controller::TimerControl() const
{
	return (Timer&)*timer_;
}

TrayIconWidget& Controller::SysTrayWidget() const
{
	return (TrayIconWidget&)*trayIcon_;
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

void Controller::SendStartTimer() {
	if(timer_)	timer_->start();
}

void Controller::SendPauseTimer() {
	if(timer_)	timer_->pause();
}

void Controller::SendStopTimer() {
	if(timer_)	timer_->stop();
}

void Controller::UpdateModel()
{
	if(dbModel_)	dbModel_->update();
}

}//namespace NS_Timer
