#include "appuserconfig.h"
#include <QQmlEngine>
#include <QJsonValue>
#include <QJsonObject>
#include "opensslwrapper.h"
#include <QFile>
#include <QFileInfo>
#include "constants.h"

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
    loadKeyPair();
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

QString AppUserConfig::getPublicKey() const
{
    return publicKey;
}

QString AppUserConfig::getPrivateKey() const
{
    return privateKey;
}

void AppUserConfig::loadKeyPair()
{
    if (!id().size())
    {
        return;
    }
    QString fileNamePublicKey(Constants::gi().getWriteablePathAnyApp() + id() + ".publicKey");
    QFile file(fileNamePublicKey);
    if (!file.exists())
    {
        OpenSSLWrapper osw;
        osw.createKeyPair();
        privateKey = osw.getPrivateKey().c_str();
        publicKey = osw.getPublicKey().c_str();
        saveKeyPair();
        return;
    }
    file.open(QFile::ReadOnly);
    publicKey = file.readAll();
    file.close();
    QString fileNamePrivateKey(Constants::gi().getWriteablePathAnyApp() + id() + ".privateKey");
    file.setFileName(fileNamePrivateKey);
    file.open(QFile::ReadOnly);
    privateKey = file.readAll();
}

void AppUserConfig::saveKeyPair()
{
    QString fileNamePublicKey(Constants::gi().getWriteablePathAnyApp() + id() + ".publicKey");
    QFile file(fileNamePublicKey);
    file.open(QFile::WriteOnly);
    file.write(publicKey);
    file.close();
    QString fileNamePrivateKey(Constants::gi().getWriteablePathAnyApp() + id() + ".privateKey");
    file.setFileName(fileNamePrivateKey);
    file.open(QFile::WriteOnly);
    file.write(privateKey);
}
