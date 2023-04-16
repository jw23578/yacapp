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
    stringFromJSON(id, Id);
    stringFromJSON(loginEMail, LoginEMail);
    stringFromJSON(loginToken, LoginToken);
    stringFromJSON(fstname, Fstname);
    stringFromJSON(surname, Surname);
    stringFromJSON(visibleName, VisibleName);
    stringFromJSON(profileImageId, ProfileImageId);
    boolFromJSON(saveLoginEMail, SaveLoginEMail);
    boolFromJSON(searchingExactlyAllowed, SearchingExactlyAllowed);
    boolFromJSON(searchingFuzzyAllowed, SearchingFuzzyAllowed);
    if (!loginEMail().size())
    {
        setLoginToken("");
    }
}

QJsonObject AppUserConfig::getConfig() const
{
    QJsonObject config;
    if (changed(id()))
    {
        stringToJSON(id);
    }
    if (changed(profileImageId()))
    {
        stringToJSON(profileImageId);
    }
    if (changed(searchingExactlyAllowed()))
    {
        boolToJSON(searchingExactlyAllowed);
    }
    if (changed(searchingFuzzyAllowed()))
    {
        boolToJSON(searchingFuzzyAllowed);
    }
    if (changed(saveLoginEMail()))
    {
        boolToJSON(saveLoginEMail);
    }
    if (changed(loginToken()))
    {
        stringToJSON(loginToken);
    }
    if (changed(loginEMail()))
    {
        stringToJSON(loginEMail);
    }
    if (changed(fstname()))
    {
        stringToJSON(fstname);
    }
    if (changed(surname()))
    {
        stringToJSON(surname);
    }
    if (changed(visibleName()))
    {
        stringToJSON(visibleName);
    }
    return config;

}

void AppUserConfig::clear()
{
    setId("");
    setFstname("");
    setSurname("");
    setVisibleName("");
    setProfileImageId("");
    setSearchingExactlyAllowed(false);
    setSearchingFuzzyAllowed(false);
    setSaveLoginEMail(false);
    setLoginEMail("");
    setLoginToken("");
    setWorkStart(QDateTime());
    setPauseStart(QDateTime());
    setOffSiteWorkStart(QDateTime());
}
