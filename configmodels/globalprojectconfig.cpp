#include "globalprojectconfig.h"
#include <QFile>
#include <QJsonValue>
#include <QJsonDocument>

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

void GlobalProjectConfig::setConfig(const QJsonValue &config)
{
    stringFromJSON(projectName, ProjectName);
    stringFromJSON(mainFormFilename, MainFormFilename);
}
