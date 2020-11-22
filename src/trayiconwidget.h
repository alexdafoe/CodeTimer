#pragma once
#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>
#include <QSharedPointer>

class Controller;

// This class in a tray widged with menu to control timer
class TrayIconWidget : public QObject
{
	Q_OBJECT
public:
	TrayIconWidget(QObject *parent, Controller* controller);
	virtual ~TrayIconWidget()							= default;

	void				setActionEnable(const QString& actionName, bool isEnabled);
	void				setTrayIcon(const QString &icon_name);

signals:
	void				signalIconActivated();
	void				signalShow();
	void				signalQuit();

private slots:
	// Show/hide main app by double click on tray icon
	void				iconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
	// Hide main app in tray by click on close button app
	void				hideIconTray();

private:
	QSharedPointer<QSystemTrayIcon>		_trayIcon;
	QSharedPointer<QMenu>					_trayMenu;
	Controller*										_controller;
};
