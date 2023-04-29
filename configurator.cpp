#include "configurator.h"
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "configmodels/globalprojectconfig.h"
#include "configmodels/parsedconfig.h"
#include "configmodels/appimagesitem.h"
#include "orm_implementions/t0027_app_images.h"

#ifndef Q_OS_ANDROID
#include "helper.h"
#endif

// #include "zlib.h"


Configurator::Configurator(YACAPP &yacApp
                           , YACExtServerNetwork &network
                           , QObject *parent)
    : QObject{parent}
    , yacApp(yacApp)
    , network(network)
{
    QStringList paths(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    if (paths.size() == 0)
    {
        return;
    }
    configFilename = paths[0] + "/yacAppConfig.json";
    qDebug() << __FILE__ << ": " << __LINE__ << configFilename;
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
        pd.setInstallationCode(config["installationCode"].toString());
        pd.setProjectName(config["projectName"].toString());
        pd.setLogoUrl(config["logoUrl"].toString());
        pd.setDeployPassword(config["deployPassword"].toString());
        pd.setDeployUser(config["deployUser"].toString());
        pd.setYacappServerLoginToken(config["yacappServerLoginToken"].toString());

        RecentProject *rp(new RecentProject);
        rp->setConfig(config);
        appendRecentProject(rp);
    }
    setRecentItemCount(recentProjectsCount());
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
        pd["projectName"] = (*it)->projectName();
        pd["installationCode"] = (*it)->installationCode();
        pd["logoUrl"] = (*it)->logoUrl();
        pd["deployPassword"] = (*it)->deployPassword();
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

void Configurator::deploy(QString globalProjectConfigFilename, QJSValue goodCallback, QJSValue badCallback)
{
    GlobalProjectConfig *gpc(new GlobalProjectConfig(true));
    gpc->init(globalProjectConfigFilename, yacApp.constants);
    if (!deployConfigs[gpc->projectID()])
    {
        deployConfigs[gpc->projectID()] = new ProjectData;
    }
    ProjectData &pd(*deployConfigs[gpc->projectID()]);
    pd.setProjectID(gpc->projectID());
    pd.setProjectName(gpc->projectName());
    pd.setLogoUrl(gpc->logoUrl());

    save();

    QFileInfo fileinfo(globalProjectConfigFilename);

    QString path(fileinfo.path() + "/");
    QString baseName(fileinfo.baseName());
    baseName.remove(" ");

    QByteArray appPackage;
    for (int i(0); i < gpc->formFiles.size(); ++i)
    {
        appPackage += gpc->formFiles[i].toUtf8();
        appPackage += '\0';
        QFile file(path + gpc->formFiles[i]);
        file.open(QIODevice::ReadOnly);
        appPackage += file.readAll();
        appPackage += '\0';
    }

    for (int i(0); i < gpc->menueFiles.size(); ++i)
    {
        appPackage += gpc->menueFiles[i].toUtf8() + '\0';
        QFile file(path + gpc->menueFiles[i]);
        file.open(QIODevice::ReadOnly);
        appPackage += file.readAll();
        appPackage += '\0';
    }
    appPackage = qCompress(appPackage);

    network.yacappServerUploadApp(pd.deployUser(),
        pd.yacappServerLoginToken(),
        gpc->projectID(),
        gpc->projectName(),
        gpc->version(),
        gpc->logoUrl(),
        gpc->appInfoUrl(),
        gpc->searchCode(),
        pd.installationCode(),
        gpc->projectColorName(),
        gpc->isTemplateApp(),
        gpc->getConfigAsString(yacApp.constants),
        appPackage.toBase64(),
        [goodCallback, this, &gpc, &pd](const QString &message) mutable
        {
            Q_UNUSED(message);
            auto &aiModel(gpc->getappImages());
            for (size_t i(0); i < aiModel.size(); ++i)
            {
                const AppImagesItem &aii(aiModel.get(i));
                QString fileName(QUrl(aii.fileUrl()).toLocalFile());
                QFile file(fileName);
                if (file.open(QFile::ReadOnly))
                {
                    QByteArray imageData(file.readAll().toBase64());
                    t0027_app_images t0027;
                    t0027.setposition(i);
                    t0027.setapp_id(gpc->projectID());
                    t0027.settransfer_image_base64(imageData);
                    network.appUserPostORM(gpc->projectID(),
                        pd.deployUser(),
                        pd.yacappServerLoginToken(),
                        t0027,
                        [](const QJsonDocument &document){},
                        [](const QString &message){});
                }
            }
            goodCallback.call();
            delete gpc;
        },
        [this, badCallback, gpc](const QString &message) mutable
        {
            delete gpc;
            if (message == "not logged in")
            {
                activeProjectData()->setYacappServerLoginToken("");
            }
            badCallback.call(QJSValueList() << message);
        });
}

void Configurator::setProjectData(const QString &projectID)
{
    if (deployConfigs.find(projectID) == deployConfigs.end())
    {
        deployConfigs[projectID] = new ProjectData;
    }
    setActiveProjectData(deployConfigs[projectID]);
}

void Configurator::yacserverLogin(const QString &loginEMail
                                  , const QString &password
                                  , const QString &projectID
                                  , QJSValue goodCallback
                                  , QJSValue badCallback)
{
    network.yacappServerLoginUser(loginEMail, password,
        [this, loginEMail, password, projectID, goodCallback](const QString &loginToken) mutable
        {
            ProjectData &pd(*deployConfigs[projectID]);
            pd.setYacappServerLoginToken(loginToken);
            pd.setDeployUser(loginEMail);
            pd.setDeployPassword(password);
            save();
            goodCallback.call(QJSValueList());
        },
        [badCallback](const QString &message) mutable
        {
            badCallback.call(QJSValueList() << message);
        }
        );
}

void Configurator::yacserverUserLoggedIn(const QString &loginEMail, const QString &loginToken, const QString &projectID)
{
    network.yacappServerUserLoggedIn(loginEMail, loginToken,
        [this, projectID](const QString &loginToken){
            deployConfigs[projectID]->setYacappServerLoginToken(loginToken);
        },
        [this, projectID](const QString &message)
        {
            Q_UNUSED(message);
            deployConfigs[projectID]->setYacappServerLoginToken("");
        }
        );
}

void Configurator::yacserverRegister(const QString &loginEMail,
                                     const QString &password,
                                     QJSValue goodCallback,
                                     QJSValue badCallback)
{
    network.yacappServerRegisterUser(loginEMail, password,
        [goodCallback](const QString &message) mutable
        {
            goodCallback.call(QJSValueList() << message);
        },
        [badCallback](const QString &message) mutable
        {
            Q_UNUSED(message);
            badCallback.call(QJSValueList() << message);
        }
        );
}

void Configurator::yacserverVerify(const QString &loginEMail
                                   , const QString &verifyToken
                                   , QJSValue goodCallback
                                   , QJSValue badCallback)
{
    network.yacappServerVerifyUser(loginEMail, verifyToken,
        [goodCallback](const QString &message) mutable
        {
            Q_UNUSED(message);
            goodCallback.call(QJSValueList());
        },
        [badCallback](const QString &message) mutable
        {
            badCallback.call(QJSValueList() << message);
        }
        );

}

bool Configurator::isFolderEmpty(const QString &folder)
{
    return QDir(QUrl(folder).path()).isEmpty();
}

void Configurator::createNewProject(const QString &projectName,
                                    const QString &projectFolder)
{
    GlobalProjectConfig gpc(false);
    gpc.setProjectName(projectName);
    gpc.setMainFormFilename("mainform.json");
    QString projectFileName(QUrl(projectFolder).path() + "/projectFile.yacapp");
    gpc.formFiles.append("mainform.json");
    gpc.save(projectFileName, yacApp.constants);

    QString mainformFileName(QUrl(projectFolder).path() + "/mainform.json");

    ParsedConfig pc;
    pc.save(mainformFileName);

    setLastProjectName(projectName);
    setLastProjectFilename(projectFileName);

    loadProjectFromFile(projectFileName);
}

void Configurator::loadProjectFromFile(const QString &projectFilename)
{
    yacApp.loadAppAndInitialize(projectFilename);
    setProjectData(yacApp.globalConfig()->projectID());
    setLastProjectFilename(projectFilename);
    setLastProjectName(yacApp.globalConfig()->projectName());
    save();
}

void Configurator::addFormFile(QString fileUrl)
{
    if (!QFile(yacApp.appFolder() + fileUrl).exists())
    {
        QFile jsonFile(yacApp.appFolder() + fileUrl);
        jsonFile.open(QIODevice::WriteOnly);
    }
    yacApp.getConfig(fileUrl);
    yacApp.globalConfig()->formFiles.append(fileUrl);
}

void Configurator::addMenueFile(QString fileUrl)
{
    if (!QFile(yacApp.appFolder() + fileUrl).exists())
    {
        QFile jsonFile(yacApp.appFolder() + fileUrl);
        jsonFile.open(QIODevice::WriteOnly);
    }
    yacApp.getMenueConfig(fileUrl);
    yacApp.globalConfig()->menueFiles.append(fileUrl);
}

void Configurator::addImageFile(QString fileUrl)
{
    qDebug() << __FILE__ << " " << fileUrl;
    AppImagesItem *aii(new AppImagesItem);
    aii->setFileUrl(fileUrl);
    aii->setId(fileUrl);
    yacApp.globalConfig()->getappImages().append(aii);
}

