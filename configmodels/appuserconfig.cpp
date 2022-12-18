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
    stringFromJSON(loginEMail, LoginEMail);
    stringFromJSON(loginToken, LoginToken);
    if (!loginEMail().size())
    {
        setLoginToken("");
    }
}

QJsonObject AppUserConfig::getConfig()
{
    QJsonObject config;
    if (changed(loginToken()))
    {
        stringToJSON(loginToken);
    }
    if (changed(loginEMail()))
    {
        stringToJSON(loginEMail);
    }
    return config;

}
