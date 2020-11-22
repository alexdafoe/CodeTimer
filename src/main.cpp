#include "log.h"
#include "controller.h"
#include "trayiconwidget.h"
#include "symbolssettings.h"
#include "timer.h"
#include "timerdata.h"
#include "databasecontroller.h"
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
	engine.rootContext()->setContextProperty("protocolLog",		&controller.LogContext());
	engine.rootContext()->setContextProperty("timer",					&controller.Timer());
	engine.rootContext()->setContextProperty("symbolsSettings",	&controller.SymbolsSettings());
	engine.rootContext()->setContextProperty("dataBase",			&controller.DatabaseController().DB());
	engine.rootContext()->setContextProperty("databaseModel",	&controller.DatabaseController().DataModel());
	engine.rootContext()->setContextProperty("sysTray",				&controller.SysTrayWidget());
	engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

	return app.exec();
}
