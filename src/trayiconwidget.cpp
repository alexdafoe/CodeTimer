#include "include/trayiconwidget.h"
#include "include/controller.h"
#include <QMenu>
#include <QIcon>
#include <QScopedPointer>
#include <QDebug>

TrayIconWidget::TrayIconWidget(QObject *parent, Controller* controller):
    QObject(parent),
    _controller(controller)
{
    _trayMenu = QSharedPointer<QMenu>(new QMenu());
    QScopedPointer <QAction> viewWindow(new QAction("Show", this));
    QScopedPointer <QAction> start(new QAction("Start", this));
    QScopedPointer <QAction> pause(new QAction("Pause", this));
    QScopedPointer <QAction> stop(new QAction("Stop", this));
    QScopedPointer <QAction> quitAct(new QAction("Quit", this));

    pause->setEnabled(false);

    connect(viewWindow.data(), &QAction::triggered, this, &TrayIconWidget::signalShow);
    connect(start.data(), &QAction::triggered, _controller, &Controller::sendStartTimer);
    connect(pause.data(), &QAction::triggered, _controller, &Controller::sendPauseTimer);
    connect(stop.data(), &QAction::triggered, _controller, &Controller::sendStopTimer);
    connect(quitAct.data(), &QAction::triggered, this, &TrayIconWidget::signalQuit);

    _trayMenu->addAction(viewWindow.take());
    _trayMenu->addSeparator();
    _trayMenu->addAction(start.take());
    _trayMenu->addAction(pause.take());
    _trayMenu->addAction(stop.take());
    _trayMenu->addSeparator();
    _trayMenu->addAction(quitAct.take());

    _trayIcon = QSharedPointer<QSystemTrayIcon>(new QSystemTrayIcon());
    _trayIcon->setContextMenu(_trayMenu.data());
    _trayIcon->setIcon(QIcon(":/images/main_icon.ico"));
    _trayIcon->setToolTip("Code Timer");

    connect(_trayIcon.data(), SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    _trayIcon->show();
}

void TrayIconWidget::setActionEnable(const QString &actionName, bool isEnabled) {
    QList<QAction*> actionList = _trayMenu->actions();
    for(auto action : actionList){
        if(action->text() == actionName){
            action->setEnabled(isEnabled);
            break;
        }
    }
}

void TrayIconWidget::setTrayIcon(const QString &icon_name) {
    _trayIcon->setIcon(QIcon(QString(":/images/%1_icon.ico").arg(icon_name)));
}

void TrayIconWidget::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch(reason){
        case QSystemTrayIcon::DoubleClick:
            emit signalIconActivated();
            break;
        default:
            break;
    }
}

void TrayIconWidget::hideIconTray() {
    _trayIcon->hide();
}
