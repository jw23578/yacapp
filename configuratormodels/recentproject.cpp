#include "recentproject.h"
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>

RecentProject::RecentProject(QObject *parent)
    : ConfigInterface{parent}
{

}

void RecentProject::setConfig(const QJsonValue &config)
{
    stringFromJSON(projectName, ProjectName);
    stringFromJSON(projectID, ProjectID);
    stringFromJSON(projectColorName, ProjectColorName);
    stringFromJSON(logoUrl, LogoUrl);
    stringFromJSON(projectFilename, ProjectFilename);
}

QJsonObject RecentProject::getConfig() const
{
    QJsonObject config;
    toJSON(logoUrl);
    toJSON(projectName);
    toJSON(projectID);
    toJSON(projectColorName);
    toJSON(projectFilename);
    return config;
}
