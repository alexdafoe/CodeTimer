#ifndef TRAYICONWIDGET_H
#define TRAYICONWIDGET_H

#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>
#include <QSharedPointer>

class Controller;

class TrayIconWidget : public QObject
{
    Q_OBJECT
public:
    TrayIconWidget(QObject *parent, Controller* controller);
    virtual ~TrayIconWidget() = default;

    void setActionEnable(const QString& actionName, bool isEnabled);
    void setTrayIcon(const QString &icon_name);

signals:
    void signalIconActivated();
    void signalShow();
    void signalQuit();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
    void hideIconTray();

private:
    QSharedPointer<QSystemTrayIcon>_trayIcon;
    QSharedPointer<QMenu> _trayMenu;
    Controller* _controller;
};

#endif // TRAYICONWIDGET_H
