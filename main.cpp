#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLocale>
#include <QTranslator>
#include "configmodels/parsedconfig.h"
#include "yacapp.h"
#include "configurator.h"
#include "configmodels/globalprojectconfig.h"
#include "configuratormodels/recentproject.h"
#include "configmodels/appuserconfig.h"
#include "projectdata.h"
#include "constants.h"
#include <QtWebView>
#include "helper.h"
#include "datamodels/messagesmodel.h"
#include "localstorage/localstorage.h"

#include "datamodels/templateddatamodel.h"
#include "dataobjects/profileobject.h"
#include "dataobjects/messageobject.h"

#include "yacappfirebase.h"

QString getAppParam(const QGuiApplication &app, const QString &param)
{
    QStringList args(app.arguments());
    for (int i(0); i < args.size(); ++i)
    {
        if (args[i].startsWith(param))
        {
            QString paramValue(args[i]);
            paramValue = paramValue.remove(0, param.size());
            return paramValue;
        }
    }
    return "";
}

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));
    QtWebView::initialize();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    app.setOrganizationName("jw78");
    app.setOrganizationDomain("jw78.de");
    qRegisterMetaType<QObjectList*>("QObjectList*");
    qmlRegisterType<MessageObject>("com.yacapp.messageobject", 1, 0, "MessageObject");
    qmlRegisterType<ParsedConfig>("com.yacapp.parsedconfig", 1, 0, "ParsedConfig");
    qmlRegisterType<BackgroundConfig>("com.yacapp.backgroundconfig", 1, 0, "BackgroundConfig");
    qmlRegisterType<MenueConfig>("com.yacapp.menueconfig", 1, 0, "MenueConfig");
    qmlRegisterType<MenueItem>("com.yacapp.menueitem", 1, 0, "MenueItem");
    qmlRegisterType<ContentConfig>("com.yacapp.contentconfig", 1, 0, "ContentConfig");
    qmlRegisterType<ContentItem>("com.yacapp.contentitem", 1, 0, "ContentItem");
    qmlRegisterType<GlobalProjectConfig>("com.yacapp.globalprojectconfig", 1, 0, "GlobalProjectConfig");
    qmlRegisterType<HeaderConfig>("com.yacapp.headerconfig", 1, 0, "HeaderConfig");
    qmlRegisterType<ProjectData>("com.yacapp.projectdata", 1, 0, "ProjectData");
    //    qmlRegisterType<ContentItem>("com.yacapp.headerc", 1, 0, "ContentItem");
    qRegisterMetaType<YACAPP*>("YACAPP");
    qRegisterMetaType<GlobalProjectConfig*>("GlobalProjectConfig");
    qRegisterMetaType<ParsedConfig*>("ParsedConfig");
    qRegisterMetaType<BackgroundConfig*>("BackgroundConfig");
    qRegisterMetaType<MenueConfig*>("MenueConfig");
    qRegisterMetaType<MenueItem*>("MenueItem");
    qRegisterMetaType<ContentConfig*>("ContentConfig");
    qRegisterMetaType<ContentItem*>("ContentItem");
    qRegisterMetaType<HeaderConfig*>("HeaderConfig");
    qRegisterMetaType<ProjectData*>("ProjectData");
    qRegisterMetaType<MessageObject*>("MessageObject");

    qmlRegisterType<RecentProject>("com.yacapp.recentproject", 1, 0, "RecentProject");
    qRegisterMetaType<RecentProject*>("RecentProject");

    qmlRegisterType<Constants>("com.yacapp.constants", 1, 0, "Constants");
    qRegisterMetaType<Constants*>("Constants");

    qmlRegisterType<AppUserConfig>("com.yacapp.appuserconfig", 1, 0, "AppUserConfig");
    qRegisterMetaType<AppUserConfig*>("AppUserConfig");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "yacapp_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    Helper helper;
    Constants constants(getAppParam(app, "customWriteablePath="));
    QNetworkAccessManager manager;
    YACExtServerNetwork network(manager
                                , constants);
    CustomServerNetwork customServerNetwork(manager
                                            , constants);

    QQmlApplicationEngine engine;
    LocalStorage localStorage(constants);
    YACAPP yacApp(engine
                  , constants
                  , helper
                  , localStorage
                  , network
                  , customServerNetwork);
    QUrl url(QStringLiteral("qrc:/main.qml"));
    Configurator *configurator(0);

    engine.rootContext()->setContextProperty("Helper", &helper);
    engine.rootContext()->setContextProperty("Constants", &constants);

    if (app.arguments().contains("Configurator"))
    {
        constants.setIsConfigurator(true);
        url = QStringLiteral("qrc:/mainDesignMode.qml");
        configurator = new Configurator(yacApp
                                        , network);
        engine.rootContext()->setContextProperty("configurator", configurator);
    }
    else
    {
        QString appFiles(getAppParam(app, "AppFiles="));
        if (appFiles.size())
        {
            yacApp.loadNewProject(appFiles);
        }
        else
        {
            // default yacApp
            yacApp.loadNewProject(yacApp.globalProjectConfigFilename());
        }
    }
    engine.rootContext()->setContextProperty("yacApp", &yacApp);

    YACAPPFirebase fb(yacApp.firebase2qt);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
