#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLocale>
#include <QTranslator>
#include "configmodels/parsedconfig.h"
#include "yacapp.h"
#include "configurator.h"
#include "configmodels/globalprojectconfig.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    qRegisterMetaType<QObjectList*>("QObjectList*");
    qmlRegisterType<ParsedConfig>("com.yacapp.parsedconfig", 1, 0, "ParsedConfig");
    qmlRegisterType<BackgroundConfig>("com.yacapp.backgroundconfig", 1, 0, "BackgroundConfig");
    qmlRegisterType<MenueConfig>("com.yacapp.menueconfig", 1, 0, "MenueConfig");
    qmlRegisterType<MenueItem>("com.yacapp.menueitem", 1, 0, "MenueItem");
    qmlRegisterType<ContentConfig>("com.yacapp.contentconfig", 1, 0, "ContentConfig");
    qmlRegisterType<ContentItem>("com.yacapp.contentitem", 1, 0, "ContentItem");
    qmlRegisterType<GlobalProjectConfig>("com.yacapp.globalprojectconfig", 1, 0, "GlobalProjectConfig");
    qmlRegisterType<HeaderConfig>("com.yacapp.headerconfig", 1, 0, "HeaderConfig");
//    qmlRegisterType<ContentItem>("com.yacapp.headerc", 1, 0, "ContentItem");
    qRegisterMetaType<ParsedConfig*>("ParsedConfig");
    qRegisterMetaType<BackgroundConfig*>("BackgroundConfig");
    qRegisterMetaType<MenueConfig*>("MenueConfig");
    qRegisterMetaType<MenueItem*>("MenueItem");
    qRegisterMetaType<ContentConfig*>("ContentConfig");
    qRegisterMetaType<ContentItem*>("ContentItem");
    qRegisterMetaType<HeaderConfig*>("HeaderConfig");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "yacapp_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    YACAPP yacApp;
    QUrl url(QStringLiteral("qrc:/main.qml"));
    Configurator *configurator(0);
    if (app.arguments().contains("Configurator"))
    {
        url = QStringLiteral("qrc:/mainDesignMode.qml");
        configurator = new Configurator;
        engine.rootContext()->setContextProperty("configurator", configurator);
    }
    else
    {
        yacApp.loadNewProject("/home/jw78/wes23/wes23.yacapp");  // hier wird aktuell die app festgelegt
    }
    engine.rootContext()->setContextProperty("yacApp", &yacApp);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
