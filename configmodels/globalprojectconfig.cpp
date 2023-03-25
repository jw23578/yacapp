#include "globalprojectconfig.h"
#include <QFile>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

GlobalProjectConfig::GlobalProjectConfig(bool noProjectId,
                                         QObject *parent)
    : ConfigInterface(parent)
{
    if (noProjectId)
    {
        return;
    }
    setProjectID(QUuid::createUuid().toString(QUuid::WithoutBraces));
}

void GlobalProjectConfig::init(QString const &jsonConfigFile,
                               Constants &constants)
{
    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument document(QJsonDocument::fromJson(fileData));
    setConfig(document["global"]);

    constants.globalDesignConfig()->setConfig(document["globalDesignConfig"]);
    constants.mainMenueConfig()->setConfig(document["mainMenueConfig"]);
}

void GlobalProjectConfig::save(const QString &jsonConfigFile,
                               Constants &constants)
{
    setVersion(version() + 1);
    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(getConfigAsString(constants));
}

void GlobalProjectConfig::setConfig(const QJsonValue &config)
{
    intFromJSON(version, Version);
    stringFromJSON(logoUrl, LogoUrl);
    doubleFromJSON(logoWidthPerThousand, LogoWidthPerThousand);
    doubleFromJSON(logoHeightPerThousand, LogoHeightPerThousand);
    doubleFromJSON(logoOffsetXPerThousand, LogoOffsetXPerThousand);
    doubleFromJSON(logoOffsetYPerThousand, LogoOffsetYPerThousand);
    stringFromJSON(projectName, ProjectName);
    stringFromJSON(projectID, ProjectID);
    stringFromJSON(projectColorName, ProjectColorName);
    boolFromJSON(isTemplateApp, IsTemplateApp);
    boolFromJSON(appUserEnabled, AppUserEnabled);
    boolFromJSON(appUserMessagesEnabled, AppUserMessagesEnabled);
    boolFromJSON(appUserWorktimeEnabled, AppUserWorktimeEnabled);
    if (projectID() == "")
    {
        setProjectID(QUuid::createUuid().toString(QUuid::WithoutBraces));
    }
    if (projectColorName() == "")
    {
        setProjectColorName("white");
    }
    stringFromJSON(mainFormFilename, MainFormFilename);
    formFiles.clear();
    QJsonArray array(config["formFiles"].toArray());
    for (int i(0); i < array.size(); ++i)
    {
        formFiles.append(array[i].toString());
    }
    menueFiles.clear();
    array = config["menueFiles"].toArray();
    for (int i(0); i < array.size(); ++i)
    {
        menueFiles.append(array[i].toString());
    }
}

QJsonObject GlobalProjectConfig::getConfig()
{
    QJsonObject config;
    intToJSON(version);
    stringToJSON(logoUrl);
    doubleToJSON(logoWidthPerThousand);
    doubleToJSON(logoHeightPerThousand);
    doubleToJSON(logoOffsetXPerThousand);
    doubleToJSON(logoOffsetYPerThousand);
    stringlistToJSON(formFiles);
    stringlistToJSON(menueFiles);
    toJSON(projectName);
    stringToJSON(projectID);
    toJSON(mainFormFilename);
    toJSON(projectColorName);
    boolToJSON(isTemplateApp);
    boolToJSON(appUserEnabled);
    boolToJSON(appUserMessagesEnabled);
    boolToJSON(appUserWorktimeEnabled);
    return config;
}

QByteArray GlobalProjectConfig::getConfigAsString(Constants &constants)
{
    QJsonObject config;
    config["global"] = getConfig();
    config["globalDesignConfig"] = constants.globalDesignConfig()->getConfig();
    config["mainMenueConfig"] = constants.mainMenueConfig()->getConfig();

    return QJsonDocument(config).toJson();
}
