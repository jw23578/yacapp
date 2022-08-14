#include "globalprojectconfig.h"
#include <QFile>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>

GlobalProjectConfig::GlobalProjectConfig(QObject *parent)
    : QObject{parent}
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
    QJsonObject global;
    getConfig(global);
    QJsonObject config;
    config["global"] = global;
    jsonFile.write(QJsonDocument(config).toJson());
}

void GlobalProjectConfig::setConfig(const QJsonValue &config)
{
    stringFromJSON(projectName, ProjectName);
    stringFromJSON(mainFormFilename, MainFormFilename);
}

void GlobalProjectConfig::getConfig(QJsonObject &config)
{
    toJSON(projectName);
    toJSON(mainFormFilename);
}
