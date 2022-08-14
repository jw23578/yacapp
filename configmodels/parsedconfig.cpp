#include "parsedconfig.h"
#include <QFile>
#include <QQmlEngine>
#include <QJsonObject>

ParsedConfig::ParsedConfig(QObject *parent):
    QObject(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

bool ParsedConfig::init(const QString &jsonConfigFile,
                        const QString &baseUrl)
{
    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    config = QJsonDocument::fromJson(fileData);

    background()->setConfig(config["background"]);
    menue()->setConfig(config["menue"], baseUrl);
    content()->setConfig(config["content"]);
    header()->setConfig(config["header"]);
    footer()->setConfig(config["footer"]);
}

void ParsedConfig::save(const QString &jsonConfigFile, const QString &baseUrl)
{
    QJsonObject config;
    config["background"] = background()->getConfig();
    config["header"] = header()->getConfig();
    config["footer"] = footer()->getConfig();
    config["content"] = content()->getConfig();
    config["menue"] = menue()->getConfig();

    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(QJsonDocument(config).toJson());
}
