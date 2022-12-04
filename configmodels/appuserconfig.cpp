#include "appuserconfig.h"
#include <QQmlEngine>
#include <QJsonValue>
#include <QJsonObject>

AppUserConfig::AppUserConfig(QObject *parent):
    ConfigInterface(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

void AppUserConfig::setConfig(const QJsonValue &config)
{
    if (config.isUndefined())
    {
        return;
    }
    stringFromJSON(loginToken, LoginToken);
}

QJsonObject AppUserConfig::getConfig()
{
    QJsonObject config;
    if (changed(loginToken()))
    {
        stringToJSON(loginToken);
    }
    return config;

}
