#include "globalprojectconfig.h"
#include <QFile>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

GlobalProjectConfig::GlobalProjectConfig(QObject *parent)
    : ConfigInterface(parent)
{
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
    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::WriteOnly);
    QJsonObject config;
    config["global"] = getConfig();
    jsonFile.write(QJsonDocument(config).toJson());
}

void GlobalProjectConfig::setConfig(const QJsonValue &config)
{
    stringFromJSON(projectName, ProjectName);
    stringFromJSON(mainFormFilename, MainFormFilename);
    QJsonArray array(config["formFiles"].toArray());
    for (int i(0); i < array.size(); ++i)
    {
        formFiles.append(array[i].toString());
    }
    array = config["menueFiles"].toArray();
    for (int i(0); i < array.size(); ++i)
    {
        menueFiles.append(array[i].toString());
    }
}

QJsonObject GlobalProjectConfig::getConfig()
{
    QJsonObject config;
    toJSON(projectName);
    toJSON(mainFormFilename);
    return config;
}
