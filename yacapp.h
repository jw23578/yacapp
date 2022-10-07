#ifndef YACAPP_H
#define YACAPP_H

#include <QObject>
#include "configmodels/parsedconfig.h"
#include "configmodels/globalprojectconfig.h"
#include "yacnetwork.h"

class YACAPP : public QObject
{
    Q_OBJECT
    QString stateFilename;
    QString projectFilename;
    YACAPPPROPERTY(QString, allAppsBaseUrl, AllAppsBaseUrl, "https://www.jw78.de/yacapp/")
    YACAPPPROPERTY(bool, isDesktop, IsDesktop, false);
    YACAPPPROPERTY(QString, baseUrl, BaseUrl, "");
    YACAPPPROPERTY(QString, loginToken, LoginToken, "");
    YACAPPPROPERTY(ParsedConfig*, mainConfig, MainConfig, 0);
    YACAPPREADONLYPROPERTY(GlobalProjectConfig*, globalConfig, new GlobalProjectConfig);
    YACAPPPROPERTY(QStringList, knownFiles, KnownFiles, QStringList())
    QMap<QString, ParsedConfig*> fileName2ParsedConfig;
    void addKnownFile(QString const &filename);

    YACAPPPROPERTY(QStringList, knownMenueFiles, KnownMenueFiles, QStringList())
    QMap<QString, MenueConfig*> fileName2MenueConfig;
    void addKnownMenueFile(QString const &filename);

    MenueConfig emptyMenue;

    YACNetwork network;

    void reset();

public:
    explicit YACAPP(QObject *parent = nullptr);
    void init(QString projectFilename);

    Q_INVOKABLE void saveState();

    Q_INVOKABLE ParsedConfig *getConfig(QString const &filename);

    Q_INVOKABLE MenueConfig *getMenueConfig(QString const &filename);

    Q_INVOKABLE void loadNewProject(QString const &projectFilename);

    Q_INVOKABLE void saveCurrentProject();

    Q_INVOKABLE void downloadApp(QString projectFilename, QString projectPackage);

signals:

    void appDownloadError(const QString &errorMessage);
};

#endif // YACAPP_H
