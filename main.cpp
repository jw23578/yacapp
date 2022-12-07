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
#include "datamodels/datamodelinterface.h"
#include "dataobjects/messageobject.h"

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    app.setOrganizationName("jw78");
    app.setOrganizationDomain("jw78.de");
    qRegisterMetaType<QObjectList*>("QObjectList*");
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
    qRegisterMetaType<GlobalProjectConfig*>("GlobalProjectConfig");
    qRegisterMetaType<ParsedConfig*>("ParsedConfig");
    qRegisterMetaType<BackgroundConfig*>("BackgroundConfig");
    qRegisterMetaType<MenueConfig*>("MenueConfig");
    qRegisterMetaType<MenueItem*>("MenueItem");
    qRegisterMetaType<ContentConfig*>("ContentConfig");
    qRegisterMetaType<ContentItem*>("ContentItem");
    qRegisterMetaType<HeaderConfig*>("HeaderConfig");
    qRegisterMetaType<ProjectData*>("ProjectData");

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
    QtWebView::initialize();
    Helper helper;
    Constants constants;
    QNetworkAccessManager manager;
    YACExtServerNetwork network(manager
                                , constants);
    CustomServerNetwork customServerNetwork(manager
                                            , constants);

    YACAPP yacApp(constants
                  , helper
                  , network
                  , customServerNetwork);
    QUrl url(QStringLiteral("qrc:/main.qml"));
    Configurator *configurator(0);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Helper", &helper);
    engine.rootContext()->setContextProperty("Constants", &constants);

    DataModelInterface messagesModel(engine, "MessagesModel", "message", DataModelInterface::reverse);
    for (size_t i(0); i < 3; ++i)
    {
        const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
        const int randomStringLength = 12;

        QString randomString(QString::number(i) + "  ");
        for(int i=0; i<randomStringLength; ++i)
        {
            int index = rand() % possibleCharacters.length();
            QChar nextChar = possibleCharacters.at(index);
            randomString.append(nextChar);
        }
        messagesModel.append(new MessageObject(randomString));
    }

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
        QString appFilesArgument("AppFiles=");
        if (app.arguments().contains(appFilesArgument))
        {
            // for development configure the app through local files
            QStringList args(app.arguments());
            for (int i(0); i < args.size(); ++i)
            {
                if (args[i].startsWith(appFilesArgument))
                {
                    QString appFiles(args[i]);
                    appFiles.remove(appFilesArgument.size());
                    yacApp.loadNewProject(appFiles);
                }
            }
        }
        else
        {
            // default yacApp
            yacApp.loadNewProject(yacApp.globalProjectConfigFilename());
        }
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
