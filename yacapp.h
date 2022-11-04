#ifndef YACAPP_H
#define YACAPP_H

#include <QObject>
#include "configmodels/parsedconfig.h"
#include "configmodels/globalprojectconfig.h"
#include "yacnetwork.h"
#include "constants.h"
#include <QJSValue>

class YACAPP : public QObject
{
    Q_OBJECT
    const Constants &constants;
    YACAPPPROPERTY(bool, isDesktop, IsDesktop, false);
    YACAPPPROPERTY(QString, baseUrl, BaseUrl, "");
    YACAPPPROPERTY(QString, loginToken, LoginToken, "");
    YACAPPPROPERTY(ParsedConfig*, mainConfig, MainConfig, 0);
    YACAPPPROPERTY(QString, globalProjectConfigFilename, GlobalProjectConfigFilename, "");
    YACAPPREADONLYPROPERTY(GlobalProjectConfig*, globalConfig, new GlobalProjectConfig);
    YACAPPPROPERTY(QStringList, knownFiles, KnownFiles, QStringList())
    QMap<QString, ParsedConfig*> fileName2ParsedConfig;
    void addKnownFile(QString const &filename);

    YACAPPPROPERTY(QStringList, knownMenueFiles, KnownMenueFiles, QStringList())
    QMap<QString, MenueConfig*> fileName2MenueConfig;
    void addKnownMenueFile(QString const &filename);

    MenueConfig emptyMenue;

    YACNetwork &network;

    void cleanUpKnownFile();

public:
    explicit YACAPP(const Constants &constants,
                    YACNetwork &network,
                    QObject *parent = nullptr);
    void init(QString projectFilename);

    Q_INVOKABLE void logout();
    Q_INVOKABLE void leaveApp();
    Q_INVOKABLE void saveState();

    Q_INVOKABLE ParsedConfig *getConfig(QString const &filename);

    Q_INVOKABLE MenueConfig *getMenueConfig(QString const &filename);

    Q_INVOKABLE void loadNewProject(QString const &projectFilename);

    Q_INVOKABLE void saveCurrentProject();

    Q_INVOKABLE void downloadApp(QString url,
                                 QString projectID);

    Q_INVOKABLE void yacappServerGetAllAPPs(QJSValue successCallback,
                                            QJSValue  errorCallback);

    Q_INVOKABLE void yacappServerGetAPP(const QString &app_id,
                                        const int current_installed_version,
                                        QJSValue successCallback,
                                        QJSValue errorCallback);

    Q_INVOKABLE void addFormFile(QString fileUrl);

signals:

    void appDownloadSuccess();
    void appDownloadError(const QString &errorMessage);
    void badMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);
    void goodMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);
    void yesNoQuestion(const QString &question, QJSValue itemToFocus, QJSValue yesCallback, QJSValue noCallback);
};

#endif // YACAPP_H
