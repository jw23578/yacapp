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


Configurator::Configurator(YACNetwork &network, QObject *parent)
    : QObject{parent},
      network(network)
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
        pd.setYacappServerLoginToken(config["yacappServerLoginToken"].toString());
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
        pd["yacappServerLoginToken"] = (*it)->yacappServerLoginToken();
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

    if (!deployConfigs[gpc.projectID()])
    {
        deployConfigs[gpc.projectID()] = new ProjectData;
    }
    ProjectData &pd(*deployConfigs[gpc.projectID()]);
    pd.setProjectID(gpc.projectID());
    pd.setDeployPassword(password);
    pd.setDeployUrl(host);
    pd.setDeployBaseDirectory("");
    pd.setDeployUser(user);

    save();

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

void Configurator::deployToYACAPPServer(const QString &globalProjectConfigFilename)
{
    GlobalProjectConfig gpc;
    gpc.init(globalProjectConfigFilename);

    if (!deployConfigs[gpc.projectID()])
    {
        deployConfigs[gpc.projectID()] = new ProjectData;
    }
    ProjectData &pd(*deployConfigs[gpc.projectID()]);
    pd.setProjectID(gpc.projectID());
    pd.setDeployBaseDirectory("");

    save();

    QFileInfo fileinfo(globalProjectConfigFilename);

    QString path(fileinfo.path() + "/");
    QString baseName(fileinfo.baseName());
    baseName.remove(" ");

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
    for (int i(0); i < gpc.menueFiles.size(); ++i)
    {
        appPackage += gpc.menueFiles[i].toUtf8() + '\0';
        QFile file(path + gpc.menueFiles[i]);
        file.open(QIODevice::ReadOnly);
        appPackage += file.readAll();
        appPackage += '\0';
    }
    appPackage = qCompress(appPackage);

    QByteArray json_yacapp; // FIXME must be filled

    network.yacappServerUploadApp(pd.deployUser(),
                                  pd.yacappServerLoginToken(),
                                  gpc.projectID(),
                                  gpc.getConfigAsString(),
                                  appPackage.toBase64(),
                                  [this](const QString &message)
    {
        deployToYACAPPServerSuccessful();
    },
    [this](const QString &message)
    {
        deployToYACAPPServerNotSuccessful(message);
    });

}

void Configurator::setProjectData(const QString &projectID)
{
    setActiveProjectData(deployConfigs[projectID]);
}

void Configurator::yacserverLogin(const QString &loginEMail, const QString &password, const QString &projectID)
{
    network.yacappServerLoginUser(loginEMail, password,
                                  [this, loginEMail, password, projectID](const QString &loginToken)
    {
        ProjectData &pd(*deployConfigs[projectID]);
        pd.setYacappServerLoginToken(loginToken);
        pd.setDeployUser(loginEMail);
        pd.setDeployPassword(password);
        save();
        loginSuccessful();
    },
    [this](const QString &message){
        loginNotSuccessful(message);
    }
    );
}

void Configurator::yacserverUserLoggedIn(const QString &loginEMail, const QString &loginToken, const QString &projectID)
{
    network.yacappServerUserLoggedIn(loginEMail, loginToken,
                                     [this, projectID](const QString &loginToken){
        deployConfigs[projectID]->setYacappServerLoginToken(loginToken);
        userLoggedInSuccessful();
    },
    [this, projectID](const QString &message){
        deployConfigs[projectID]->setYacappServerLoginToken("");
        userLoggedInNotSuccessful();
    }
    );
}

void Configurator::yacserverRegister(const QString &loginEMail, const QString &password)
{
    network.yacappServerRegisterUser(loginEMail, password,
                                     [this](const QString &message){
        registerSuccessful();
    },
    [this](const QString &message){
        registerNotSuccessful(message);
    }
    );
}

void Configurator::yacserverVerify(const QString &loginEMail, const QString &verifyToken)
{
    network.yacappServerVerifyUser(loginEMail, verifyToken,
                                   [this](const QString &message){
        verifySuccessful();
    },
    [this](const QString &message){
        verifyNotSuccessful(message);
    }
    );

}
