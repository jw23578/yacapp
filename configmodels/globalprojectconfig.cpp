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

void GlobalProjectConfig::init(QString const &jsonConfigFile)
{
    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    setConfig(QJsonDocument::fromJson(fileData)["global"]);
}

void GlobalProjectConfig::save(const QString &jsonConfigFile)
{
    setVersion(version() + 1);
    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(getConfigAsString());
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
    return config;
}

QByteArray GlobalProjectConfig::getConfigAsString()
{
    QJsonObject config;
    config["global"] = getConfig();
    return QJsonDocument(config).toJson();
}
