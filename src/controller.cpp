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
: appPath_(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/CodeTimer/")
{
	QDir dir;
	if(!dir.exists(appPath_))
		dir.mkpath(appPath_);
	try {
		logContext_			= std::make_shared<LogContext>			(new LogContext(appPath_));
		dbModel_			= std::make_shared<DatabaseModel>		(new DatabaseModel(this));
		dbWrap_				= std::make_shared<DatabaseWrap>		(new DatabaseWrap(dbModel_, appPath_));
		eventFilter_			= std::make_shared<KeyEventFilter>		(new KeyEventFilter(this));
		symbolsSettings_	= std::make_shared<SymbolsSettings>	(new SymbolsSettings(eventFilter_));
		timer_				= std::make_shared<Timer>					(new Timer(*this));
		trayIcon_			= std::make_shared<TrayIconWidget>		(new TrayIconWidget(this));
	} catch (std::exception ex) {
		qWarning() << ex.what();
		QMessageBox msg;
		msg.setIcon(QMessageBox::Critical);
		msg.setText(ex.what());
		msg.setStandardButtons(QMessageBox::Ok);
		msg.exec();
	}
}

Controller::~Controller() {
	qDebug() << "shutdown application";
}

void Controller::Init() {
	logContext_->Init();
	// init key event filter
	symbolsSettings_->SymbolListChanged(); // load to KeyEventFiler symbols for tracking

	// init database
	dbWrap_->ConnectToDB();
	dbModel_->Update();
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

void Controller::SendStartTimer(){
	if(timer_)	timer_->Start();
}

void Controller::SendPauseTimer() {
	if(timer_)	timer_->Pause();
}

void Controller::SsendStopTimer() {
	if(timer_)	timer_->Stop();
}

void Controller::UpdateModel()
{
	if(dbModel_)	dbModel_->Update();
}

}//namespace NS_Timer
