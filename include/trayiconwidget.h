#ifndef TRAYICONWIDGET_H
#define TRAYICONWIDGET_H

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
    // Constructor
    TrayIconWidget(QObject *parent, Controller* controller);
    // Destructor
    virtual ~TrayIconWidget() = default;

    // Activate/deactivate item in tray widget menu
    void setActionEnable(const QString& actionName, bool isEnabled);
    // Change tray icon in dependence timer state
    void setTrayIcon(const QString &icon_name);

signals:
    void signalIconActivated();
    void signalShow();
    void signalQuit();

private slots:
    // Show/hide main app by double click on tray icon
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
    // Hide main app in tray by click on close button app
    void hideIconTray();

private:
    QSharedPointer<QSystemTrayIcon>_trayIcon;
    QSharedPointer<QMenu> _trayMenu;
    Controller* _controller;
};

#endif // TRAYICONWIDGET_H
