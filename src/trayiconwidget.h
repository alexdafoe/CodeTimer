#pragma once
#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>
#include <QSharedPointer>

namespace NS_Timer
{

class Controller;

// This class in a tray widged with menu to control timer
class TrayIconWidget : public QObject
{
	Q_OBJECT
public:
	TrayIconWidget(QObject* parent, Controller*);
	virtual ~TrayIconWidget()							= default;

	void				SetActionEnable(const QString& actionName, bool isEnabled);
	void				SetTrayIcon(const QString& icon_name);

signals:
	void				SignalIconActivated();
	void				SignalShow();
	void				SignalQuit();

private slots:
	// Show/hide main app by double click on tray icon
	void				IconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
	// Hide main app in tray by click on close button app
	void				HideIconTray();

private:
	QSharedPointer<QSystemTrayIcon>		trayIcon_;
	QSharedPointer<QMenu>					trayMenu_;
	Controller*										controller_;
};

}//namespace NS_Timer
