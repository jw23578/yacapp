#include "configurator.h"
#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "jw78curlwrapper.h"


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

void Configurator::deploy(QString host, QString user, QString password, QString www_basedirectory)
{
    std::string remoteUrl("sftp://");
    remoteUrl += user.toStdString();
    remoteUrl += ":";
    remoteUrl += password.toStdString();
    remoteUrl += "@";
    remoteUrl += host.toStdString();
    remoteUrl += ":";
    remoteUrl += www_basedirectory.toStdString();

    jw78::CurlWrapper cw;
    std::string message;
    cw.sftpUpload(remoteUrl, "/home/jw78/wes23/wes23.yacapp", message);
}
