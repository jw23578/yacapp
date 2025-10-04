#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "logger.h"

#include <QLocale>
#include <QTranslator>
#include "configmodels/parsedconfig.h"
#include "yacapp.h"
#include "configurator.h"
#include "configmodels/globalprojectconfig.h"
#include "configuratormodels/recentproject.h"
#include "configmodels/appuserconfig.h"
#include "configmodels/appimagesitem.h"
#include "projectdata.h"
#include "constants.h"
#include "configuratorconstants.h"
#include <QtWebView>
#include "helper.h"
#include "datamodels/messagesmodel.h"
#include "datamodels/transmissiontracker.h"
#include "localstorage/localstorage.h"

#include "dataobjects/profileobject.h"
#include "dataobjects/messageobject.h"

#include "yacappfirebase.h"
#include "cppqmlobjects/thirdpartylogin.h"

#include "opensslwrapper.h"

#ifndef DISABLEQZXING
#include "QZXing.h"
#endif

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
//    OpenSSLWrapper osw;
//    osw.createKeyPair();
//    std::cout << osw.getPrivateKey() << std::endl << std::endl;
//    std::cout << osw.getPublicKey() << std::endl << std::endl;

//    OpenSSLWrapper osw2;
//    osw2.loadPrivateKey(osw.getPrivateKey());

//    OpenSSLWrapper osw3;
//    osw3.loadPublicKey(osw.getPublicKey());
//    //    std::cout << osw2.getPrivateKey() << std::endl << std::endl;
//    //    std::cout << osw2.getPublicKey() << std::endl << std::endl;

//    std::string original("Hallo lieber Jens, ich bin gespannt, ob das klappt. Noch etwas Text und noch mehr mehr mehr");
//    std::vector<unsigned char> encryptedMessage;

//    std::vector<unsigned char> encryptedKey;
//    std::vector<unsigned char> initialiationVector;
//    osw3.encrypt(original, encryptedMessage, encryptedKey, initialiationVector);
//    std::cout << "encrypted.size(): " << encryptedMessage.size() << std::endl;
//    std::string resultMessage;
//    osw2.decrypt(encryptedMessage, resultMessage, encryptedKey, initialiationVector);
//    std::cout << resultMessage << std::endl << std::endl;
//    return 0;

    Logger::gi().setLogFile("/home/jw78/yacapplog.txt");
    DEFAULT_LOG("yacapp start, not yet known if app or configurator");
    QDateTime begin(QDateTime::currentDateTime());
    QString b1 = begin.toTimeSpec(Qt::OffsetFromUTC).toString(Qt::ISODate);
    QString b2 = begin.toTimeSpec(Qt::LocalTime).toString(Qt::ISODate);
    QString b3 = begin.toTimeSpec(Qt::TimeZone).toString(Qt::ISODate);
    QString b4 = begin.toTimeSpec(Qt::OffsetFromUTC).toString(Qt::ISODate);

    //    return 0;
    std::srand(std::time(nullptr));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QtWebView::initialize();
    QGuiApplication app(argc, argv);
    app.setOrganizationName("jw78");
    app.setOrganizationDomain("jw78.de");

#ifndef DISABLEQZXING
    QZXing::registerQMLTypes();
#endif

    qRegisterMetaType<QObjectList*>("QObjectList*");
    qmlRegisterType<TransmissionTracker>("com.yacapp.transmissiontracker", 1,  0, "TransmissionTracker");
    qmlRegisterType<MessageObject>("com.yacapp.messageobject", 1, 0, "MessageObject");
    qmlRegisterType<ParsedConfig>("com.yacapp.parsedconfig", 1, 0, "ParsedConfig");
    qmlRegisterType<BackgroundConfig>("com.yacapp.backgroundconfig", 1, 0, "BackgroundConfig");
    qmlRegisterType<MenueConfig>("com.yacapp.menueconfig", 1, 0, "MenueConfig");
    qmlRegisterType<AppImagesItem>("com.yacapp.appimagesitem", 1, 0, "AppImagesItem");
    qmlRegisterType<MenueItem>("com.yacapp.menueitem", 1, 0, "MenueItem");
    qmlRegisterType<ContentConfig>("com.yacapp.contentconfig", 1, 0, "ContentConfig");
    qmlRegisterType<ContentItem>("com.yacapp.contentitem", 1, 0, "ContentItem");
    qmlRegisterType<GlobalProjectConfig>("com.yacapp.globalprojectconfig", 1, 0, "GlobalProjectConfig");
    qmlRegisterType<HeaderConfig>("com.yacapp.headerconfig", 1, 0, "HeaderConfig");
    qmlRegisterType<ProjectData>("com.yacapp.projectdata", 1, 0, "ProjectData");
    qRegisterMetaType<YACAPP*>("YACAPP");
    qRegisterMetaType<TransmissionTracker*>("TransmissionTracker");
    qRegisterMetaType<GlobalProjectConfig*>("GlobalProjectConfig");
    qRegisterMetaType<AppImagesItem*>("AppImagesItem");
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
    QString customWriteablePath(getAppParam(app, "customWriteablePath="));
    if (app.arguments().contains("ServerIsLocalhost"))
    {
        YACServerBaseNetwork::yacappServerUrl = "http://127.0.0.1:23578";
        customWriteablePath = "/home/jw78/MyYacApps/LocalYACAPPConfig";
    }
    if (app.arguments().contains("Other"))
    {
        customWriteablePath = "/home/jw78/MyYacApps/LocalYACAPPConfigOther";
    }
    if (app.arguments().contains("ServerIsLocalhostOther"))
    {
        YACServerBaseNetwork::yacappServerUrl = "http://127.0.0.1:23578";
        customWriteablePath = "/home/jw78/MyYacApps/LocalYACAPPConfigOther";
    }
    Helper helper;
    CPPQMLAppAndConfigurator cppQMLAppAndConfigurator;
    ThirdPartyLogin thirdPartyLogin;
    Constants::gi(customWriteablePath);
    Logger::gi().setIsDesktop(Constants::gi().isDesktop());

    QNetworkAccessManager manager;
    YACExtServerNetworkDeprecated network(manager
                                , Constants::gi());
    CustomServerNetwork customServerNetwork(manager
                                            , Constants::gi());

    QQmlApplicationEngine engine;
    YACAPP *yacApp(new YACAPP(engine
                              , cppQMLAppAndConfigurator
                              , thirdPartyLogin
                              , Constants::gi()
                              , helper
                              , network
                              , customServerNetwork));
    QUrl url(QStringLiteral("qrc:/main.qml"));
    Configurator *configurator(0);

    engine.rootContext()->setContextProperty("Helper", &helper);
    engine.rootContext()->setContextProperty("Constants", &Constants::gi());
    engine.rootContext()->setContextProperty("CPPQMLAppAndConfigurator", &cppQMLAppAndConfigurator);
    engine.rootContext()->setContextProperty("ThirdPartyLogin", &thirdPartyLogin);

    if (app.arguments().contains("Configurator"))
    {
        ConfiguratorConstants *cc(new ConfiguratorConstants(0));
        engine.rootContext()->setContextProperty("ConfiguratorConstants", cc);
        Constants::gi().setFontColor(cc->textPrimaryColor());
        Constants::gi().setLineEditBackgroundColor(cc->inputBackgroundColor());
        Constants::gi().setTextInputFontColor(cc->inputTextColor());
        Constants::gi().setButtonColor(cc->buttonPrimaryColor());
        Constants::gi().setButtonHoverColor(cc->buttonPrimaryHoverColor());
        Constants::gi().setDark(true);
        Constants::gi().setIsConfigurator(true);
        url = QStringLiteral("qrc:/mainCreatorMode.qml");
        configurator = new Configurator(*yacApp
                                        , helper
                                        , cppQMLAppAndConfigurator
                                        , network);
        engine.rootContext()->setContextProperty("configurator", configurator);
    }
    else
    {
        QString appFiles(getAppParam(app, "AppFiles="));
        if (appFiles.size())
        {
            yacApp->loadAppAndInitialize(appFiles);
        }
        else
        {
            // default yacApp
            yacApp->loadAppAndInitialize(yacApp->globalProjectConfigFilename());
        }
    }
    int noAutoFetchMessages(getAppParam(app, "noAutoFetchMessages=").toInt());
    if (noAutoFetchMessages)
    {
        yacApp->setAutoFetchMessages(false);
    }
    engine.rootContext()->setContextProperty("yacApp", yacApp);

    YACAPPFirebase fb(yacApp->firebase2qt);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    int result(app.exec());
    return result;
}
