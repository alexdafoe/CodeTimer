#include "trayiconwidget.h"
#include "controller.h"
#include <QMenu>
#include <QIcon>
#include <QScopedPointer>
#include <QDebug>

namespace NS_Timer
{

TrayIconWidget::TrayIconWidget(Controller* _controller)
: controller_(_controller)
{
	trayMenu_ = QSharedPointer<QMenu>(new QMenu());
	QScopedPointer <QAction> viewWindow(new QAction("Show", this));
	QScopedPointer <QAction> start(new QAction("Start", this));
	QScopedPointer <QAction> pause(new QAction("Pause", this));
	QScopedPointer <QAction> stop(new QAction("Stop", this));
	QScopedPointer <QAction> quitAct(new QAction("Quit", this));

	pause->setEnabled(false);

	connect(viewWindow.data(), &QAction::triggered, this, &TrayIconWidget::SignalShow);
	connect(start.data(), &QAction::triggered, controller_, &Controller::SendStartTimer);
	connect(pause.data(), &QAction::triggered, controller_, &Controller::SendPauseTimer);
	connect(stop.data(), &QAction::triggered, controller_, &Controller::SsendStopTimer);
	connect(quitAct.data(), &QAction::triggered, this, &TrayIconWidget::SignalQuit);

	trayMenu_->addAction(viewWindow.take());
	trayMenu_->addSeparator();
	trayMenu_->addAction(start.take());
	trayMenu_->addAction(pause.take());
	trayMenu_->addAction(stop.take());
	trayMenu_->addSeparator();
	trayMenu_->addAction(quitAct.take());

	trayIcon_ = QSharedPointer<QSystemTrayIcon>(new QSystemTrayIcon());
	trayIcon_->setContextMenu(trayMenu_.data());
	trayIcon_->setIcon(QIcon(":/images/main_icon.ico"));
	trayIcon_->setToolTip("Code Timer");

	connect(trayIcon_.data(), SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
				this, SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));
	trayIcon_->show();
}

void TrayIconWidget::SetActionEnable(const QString& _actionName, bool _isEnabled) {
	QList<QAction*> actionList = trayMenu_->actions();
	for(auto action : actionList){
		if(action->text() == _actionName){
			action->setEnabled(_isEnabled);
			break;
		}
	}
}

void TrayIconWidget::SetTrayIcon(const QString& _iconName) {
	trayIcon_->setIcon(QIcon(QString(":/images/%1_icon.ico").arg(_iconName)));
}

void TrayIconWidget::IconActivated(QSystemTrayIcon::ActivationReason _reason) {
	switch(_reason){
		case QSystemTrayIcon::DoubleClick:
			emit SignalIconActivated();
			break;
		default:
			break;
	}
}

void TrayIconWidget::HideIconTray() {
	trayIcon_->hide();
}

}//namespace NS_Timer
