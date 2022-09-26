#include "configurator.h"
#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

Configurator::Configurator(QObject *parent)
    : QObject{parent}
{
    QStringList paths(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    if (paths.size() == 0)
    {
        return;
    }
    configFilename = paths[0] + "/yacAppConfig.json";
    QFile jsonFile(configFilename);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument config(QJsonDocument::fromJson(fileData));
    stringFromJSON(lastProjectName, LastProjectName);
    stringFromJSON(lastProjectFilename, LastProjectFilename);
}

void Configurator::save()
{
    QJsonObject config;
    stringToJSON(lastProjectName);
    stringToJSON(lastProjectFilename);
    QFile jsonFile(configFilename);
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(QJsonDocument(config).toJson());

}
