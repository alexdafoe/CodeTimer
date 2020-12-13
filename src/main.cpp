
#include "controller.h"
#include "log.h"
#include "trayiconwidget.h"
#include "symbolssettings.h"
#include "timer.h"
#include "timerdata.h"
#include "databasemodel.h"
#include "database.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/images/main_icon.ico"));

	NS_Timer::Controller controller;
	controller.Init();

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("logContext",			&controller.Log());
	engine.rootContext()->setContextProperty("timer",					&controller.TimerControl());
	engine.rootContext()->setContextProperty("symbolsSettings",	&controller.KeyControlSettings());
	engine.rootContext()->setContextProperty("database",			&controller.DB());
	engine.rootContext()->setContextProperty("databaseModel",	&controller.DBModel());
	engine.rootContext()->setContextProperty("sysTray",				&controller.SysTrayWidget());
	engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

	return app.exec();
}
