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
}

QJsonObject RecentProject::getConfig()
{
    QJsonObject config;
    stringToJSON(logoUrl);
    toJSON(projectName);
    stringToJSON(projectID);
    toJSON(projectColorName);
    return config;
}
