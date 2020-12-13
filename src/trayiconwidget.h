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
	explicit TrayIconWidget(Controller*);
	TrayIconWidget(const TrayIconWidget&)							= delete;
	TrayIconWidget(TrayIconWidget&&)								= delete;
	virtual ~TrayIconWidget()												= default;

	TrayIconWidget&	operator=(const TrayIconWidget&)			= delete;
	TrayIconWidget&	operator=(TrayIconWidget&&)				= delete;

	void					SetActionEnable(const QString& actionName, bool isEnabled);
	void					SetTrayIcon(const QString& icon_name);

signals:
	void					signalIconActivated();
	void					signalShow();
	void					signalQuit();

private slots:
	// Show/hide main app by double click on tray icon
	void					IconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
	// Hide main app in tray by click on close button app
	void					hideIconTray();

private:
	Controller*										controller_		= nullptr;
	QSharedPointer<QSystemTrayIcon>		trayIcon_;
	QSharedPointer<QMenu>					trayMenu_;
};

}//namespace NS_Timer
