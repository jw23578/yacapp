#include "configurator.h"
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include "jw78curlwrapper.h"
#include <QJsonArray>
#include "configmodels/globalprojectconfig.h"
#include "zlib.h"


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

    QJsonArray dc(config["deployConfigs"].toArray());
    for (int i(0); i < dc.size(); ++i)
    {
        QJsonValue config(dc[i]);
        QString projectID(config["projectID"].toString());
        deployConfigs[projectID] = new ProjectData;
        ProjectData &pd(*deployConfigs[projectID]);
        pd.setProjectID(projectID);
        pd.setDeployPassword(config["deployPassword"].toString());
        pd.setDeployUrl(config["deployUrl"].toString());
        pd.setDeployBaseDirectory(config["deployBaseDirectory"].toString());
        pd.setDeployUser(config["deployUser"].toString());
    }
}

void Configurator::save()
{
    QJsonObject config;
    stringToJSON(lastProjectName);
    stringToJSON(lastProjectFilename);
    QJsonArray dc;
    QMap<QString, ProjectData*>::Iterator it(deployConfigs.begin());
    while (it != deployConfigs.end())
    {
        QJsonObject pd;
        pd["projectID"] = (*it)->projectID();
        pd["deployPassword"] = (*it)->deployPassword();
        pd["deployUrl"] = (*it)->deployUrl();
        pd["deployBaseDirectory"] = (*it)->deployBaseDirectory();
        pd["deployUser"] = (*it)->deployUser();
        ++it;
        dc.append(pd);
    }
    config["deployConfigs"] = dc;
    QFile jsonFile(configFilename);
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(QJsonDocument(config).toJson());

}

void Configurator::deploy(QString projectID, QString host, QString user, QString password, QString www_basedirectory)
{
    if (!deployConfigs[projectID])
    {
        deployConfigs[projectID] = new ProjectData;
    }
    ProjectData &pd(*deployConfigs[projectID]);
    pd.setProjectID(projectID);
    pd.setDeployPassword(password);
    pd.setDeployUrl(host);
    pd.setDeployBaseDirectory(www_basedirectory);
    pd.setDeployUser(user);

    save();

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

void Configurator::sftpUpload(QString host, QString user, QString password, QString targetFilename, QString sourceFilename)
{
    std::string remoteUrl("sftp://");
    remoteUrl += user.toStdString();
    remoteUrl += ":";
    remoteUrl += password.toStdString();
    remoteUrl += "@";
    remoteUrl += host.toStdString();
    remoteUrl += ":";
    remoteUrl += targetFilename.toStdString();

    jw78::CurlWrapper cw;
    std::string message;
    cw.sftpUpload(remoteUrl, sourceFilename.toStdString(), message);
}


void Configurator::defaultDeploy(const QString &globalProjectConfigFilename, QString host, QString user, QString password)
{
    GlobalProjectConfig gpc;
    gpc.init(globalProjectConfigFilename);

    QFileInfo fileinfo(globalProjectConfigFilename);

    QString path(fileinfo.path() + "/");
    QString baseName(fileinfo.baseName());
    baseName.remove(" ");

    sftpUpload(host, user, password, QString("/var/www/html/yacapp/") + gpc.projectID() + ".yacapp",
               globalProjectConfigFilename);

    QByteArray appPackage;
    for (int i(0); i < gpc.formFiles.size(); ++i)
    {
        appPackage += gpc.formFiles[i].toUtf8();
        appPackage += '\0';
        QFile file(path + gpc.formFiles[i]);
        file.open(QIODevice::ReadOnly);
        appPackage += file.readAll();
        appPackage += '\0';
    }
    appPackage += "MenueFiles";
    appPackage += '\0';
    for (int i(0); i < gpc.menueFiles.size(); ++i)
    {
        appPackage += gpc.menueFiles[i].toUtf8() + '\0';
        QFile file(path + gpc.menueFiles[i]);
        file.open(QIODevice::ReadOnly);
        appPackage += file.readAll();
        appPackage += '\0';
    }
    appPackage = qCompress(appPackage);

    QString appPackageFilename(path + gpc.projectID() + ".yacpck");
    QFile appPackageFile(appPackageFilename);
    appPackageFile.open(QIODevice::WriteOnly);
    appPackageFile.write(appPackage);
    appPackageFile.close();

    sftpUpload(host, user, password, QString("/var/www/html/yacapp/") + gpc.projectID() + ".yacpck", appPackageFilename);
}

ProjectData *Configurator::getProjectData(const QString &projectID)
{
    return deployConfigs[projectID];
}
