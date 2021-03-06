#include "include/log.h"
#include "include/controller.h"
#include "include/trayiconwidget.h"
#include "include/symbolssettings.h"
#include "include/timer.h"
#include "include/timerdata.h"
#include "include/database/databasecontroller.h"
#include "include/database/databasemodel.h"
#include "include/database/database.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/main_icon.ico"));

    Controller controller;
    controller.init();
    SymbolsSettings *settings = controller.getSymbolsSettings();
    Log* log = controller.getLog();
    Timer *timer = controller.getTimer();
    DataBase* dataBase = controller.getDataBaseController()->getDataBase();
    DataBaseModel * dataModel = controller.getDataBaseController()->getDataModel();
    TrayIconWidget *sysTray = controller.getSysTrayWidget();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("protocolLog", log);
    engine.rootContext()->setContextProperty("timer", timer);
    engine.rootContext()->setContextProperty("symbolsSettings", settings);
    engine.rootContext()->setContextProperty("dataBase", dataBase);
    engine.rootContext()->setContextProperty("databaseModel", dataModel);
    engine.rootContext()->setContextProperty("sysTray", sysTray);
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    return app.exec();
}
