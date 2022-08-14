#include "parsedconfig.h"
#include <QFile>
#include <QQmlEngine>

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
