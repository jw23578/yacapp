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
#include "logger.h"
#include "orm_implementions/t0001_apps.h"

#ifndef Q_OS_ANDROID
#include "helper.h"
#endif

// #include "zlib.h"


Configurator::Configurator(YACAPP &yacApp
                           , Helper &helper
                           , CPPQMLAppAndConfigurator &cppQMLAppAndConfigurator
                           , YACExtServerNetworkDeprecated &network
                           , QObject *parent)
    : QObject{parent}
    , yacApp(yacApp)
    , helper(helper)
    , cppQMLAppAndConfigurator(cppQMLAppAndConfigurator)
    , network(network)
    , yacAppConfiguratorConfig("/yacAppConfiguratorConfig.json")
{
    QStringList paths(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    if (paths.size() == 0)
    {
        return;
    }
    configFilename = paths[0] + yacAppConfiguratorConfig;
    DEFAULT_LOG_VARIABLE(configFilename);
    QFile jsonFile(configFilename);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument config(QJsonDocument::fromJson(fileData));
    stringFromJSON(lastProjectName, LastProjectName);
    stringFromJSON(lastProjectFilename, LastProjectFilename);
    stringFromJSON(lastProjectLogoUrl, LastProjectLogoUrl);
    stringFromJSON(yacappServerLoginToken, YacappServerLoginToken);
    stringFromJSON(deployUser, DeployUser);

    QJsonArray dc(config["deployConfigs"].toArray());
    for (int i(0); i < dc.size(); ++i)
    {
        QJsonValue config(dc[i]);
        QString projectID(config["projectID"].toString());
        if (projectID.size())
        {
            deployConfigs[projectID] = new ProjectData;
            ProjectData &pd(*deployConfigs[projectID]);
            pd.setProjectID(projectID);
            pd.setInstallationCode(config["installationCode"].toString());
            pd.setProjectName(config["projectName"].toString());
            pd.setProjectFilename(config["projectFilename"].toString());
            pd.setLogoUrl(config["logoUrl"].toString());
            pd.setThird(config["third"].toString());
            pd.setMandant(config["mandant"].toString());

            RecentProject *rp(new RecentProject);
            rp->setConfig(config);
            appendRecentProject(rp);
        }
    }
    setRecentItemCount(recentProjectsCount());
}

void Configurator::save()
{
    setLastProjectName(yacApp.globalConfig()->projectName());
    setLastProjectLogoUrl(yacApp.globalConfig()->logoUrl());

    QJsonObject config;
    toJSON(lastProjectName);
    toJSON(lastProjectFilename);
    toJSON(lastProjectLogoUrl);
    toJSON(yacappServerLoginToken);
    toJSON(deployUser);
    QJsonArray dc;
    QMap<QString, ProjectData*>::Iterator it(deployConfigs.begin());
    while (it != deployConfigs.end())
    {
        QJsonObject pd;
        pd["projectID"] = (*it)->projectID();
        pd["projectName"] = (*it)->projectName();
        pd["projectFilename"] = (*it)->projectFilename();
        pd["installationCode"] = (*it)->installationCode();
        pd["logoUrl"] = (*it)->logoUrl();
        pd["third"] = (*it)->third();
        pd["mandant"] = (*it)->mandant();
        ++it;
        dc.append(pd);
    }
    config["deployConfigs"] = dc;
    QFile jsonFile(configFilename);
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(QJsonDocument(config).toJson());

}

void Configurator::deploy(QJSValue goodCallback, QJSValue badCallback)
{
    GlobalProjectConfig *gpc(new GlobalProjectConfig(true));
    gpc->init(yacApp.globalProjectConfigFilename(),
              yacApp.constants);
    if (!deployConfigs[gpc->projectID()])
    {
        deployConfigs[gpc->projectID()] = new ProjectData;
    }
    ProjectData &pd(*deployConfigs[gpc->projectID()]);
    pd.setProjectID(gpc->projectID());
    pd.setProjectName(gpc->projectName());
    pd.setProjectFilename(lastProjectFilename());
    pd.setLogoUrl(gpc->logoUrl());
    pd.setThird(gpc->third());
    pd.setMandant(gpc->mandant());

    save();

    QFileInfo fileinfo(yacApp.globalProjectConfigFilename());

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

    t0001_apps theApp;
    theApp.setid(gpc->projectID());
    theApp.setapp_name(gpc->projectName());
    theApp.setapp_version(gpc->version());
    theApp.setapp_logo_url(gpc->logoUrl());
    theApp.setapp_info_url(gpc->appInfoUrl());
    theApp.setsearch_code(gpc->searchCode());
    theApp.setapp_color_name(gpc->projectColorName());
    theApp.setis_template_app(gpc->isTemplateApp());
    theApp.setyacpck_base64(0);
    theApp.settransfer_yacpck_base64(gpc->getConfigAsString(yacApp.constants));
    theApp.setjson_yacapp(appPackage.toBase64());

    network.yacappServerUploadApp(deployUser(),
                                  yacappServerLoginToken(),
                                  theCreatorAppAppId,
                                  theApp,
                                  pd.installationCode(),
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
                                                                     deployUser(),
                                                                     yacappServerLoginToken(),
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
                                          setYacappServerLoginToken("");
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
                                  , QJSValue goodCallback
                                  , QJSValue badCallback)
{
    network.loginUser(loginEMail,
                      password,
                      theCreatorAppAppId,
                      "",
                      [this, loginEMail, goodCallback](const QJsonDocument &jsonDoc) mutable
                      {
                          QJsonObject object(jsonDoc.object());
                          QString loginToken(object["loginToken"].toString());
                          setYacappServerLoginToken(loginToken);
                          setDeployUser(loginEMail);
                          save();
                          goodCallback.call(QJSValueList());
                      },
                      [badCallback](const QString &message) mutable
                      {
                          badCallback.call(QJSValueList() << message);
                      }
                      );
}

void Configurator::yacserverLogout(const QString &loginEMail,
                                   const QString &loginToken,
                                   QJSValue goodCallback,
                                   QJSValue badCallback)
{
    network.logoutUser(loginEMail,
                       loginToken,
                       theCreatorAppAppId,
                       [this, goodCallback](const QString &message) mutable
                       {
                           setYacappServerLoginToken("");
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
    network.userLoggedIn(loginEMail,
                         loginToken,
                         [this, projectID](const QString &loginToken){
                             setYacappServerLoginToken(loginToken);
                         },
                         [this, projectID](const QString &message)
                         {
                             Q_UNUSED(message);
                             setYacappServerLoginToken("");
                         }
                         );
}

void Configurator::yacserverRegister(const QString &loginEMail,
                                     const QString &password,
                                     QJSValue goodCallback,
                                     QJSValue badCallback)
{
    network.registerUser(loginEMail,
                         password,
                         theCreatorAppAppId,
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

void Configurator::requestVerifyTokenCreator(const QString &loginEMail,
                                             QJSValue goodCallback,
                                             QJSValue badCallback)
{
    network.yacappServerUserRequestVerifyToken(loginEMail,
                                               theCreatorAppAppId,
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

void Configurator::verifyCreator(const QString &loginEMail
                                 , const QString &verifyToken
                                 , QJSValue goodCallback
                                 , QJSValue badCallback)
{
    network.yacappServerUserVerify(loginEMail,
                                   verifyToken,
                                   theCreatorAppAppId,
                                   [this, loginEMail, goodCallback](const QJsonDocument &jsonDoc) mutable
                                   {
                                       QJsonObject object(jsonDoc.object());
                                       QString loginToken(object["loginToken"].toString());
                                       setYacappServerLoginToken(loginToken);
                                       setDeployUser(loginEMail);
                                       save();
                                       goodCallback.call(QJSValueList() << "User verified and logged in.");
                                   },
                                   [badCallback](const QString &message) mutable
                                   {
                                       badCallback.call(QJSValueList() << message);
                                   }
                                   );

}

void Configurator::createNewProject(const QString &projectName
                                    , const QString &projectFolder
                                    , QJSValue okCallback
                                    , QJSValue projectNameEdit
                                    , QJSValue projectFolderEdit)
{
    if (projectName == "")
    {
        emit cppQMLAppAndConfigurator.badMessage(tr("Please enter the Projectname"), projectNameEdit, QJSValue::NullValue);
        return;
    }
    if (projectFolder == "" || !helper.isFolderEmpty(projectFolder))
    {
        emit cppQMLAppAndConfigurator.badMessage(tr("Please select an empty folder for your new project."), projectFolderEdit, QJSValue::NullValue);
        return;
    }


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

    okCallback.call();
}

void Configurator::loadProjectFromFile(const QString &projectFilename)
{
    yacApp.loadAppAndInitialize(projectFilename);
    setProjectData(yacApp.globalConfig()->projectID());
    setLastProjectFilename(projectFilename);
    save();
}

void Configurator::saveCurrentProject()
{
    yacApp.saveAppToFile(yacApp.globalProjectConfigFilename());
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
    DEFAULT_LOG_VARIABLE(fileUrl);
    AppImagesItem *aii(new AppImagesItem);
    aii->setFileUrl(fileUrl);
    aii->setId(fileUrl);
    yacApp.globalConfig()->getappImages().append(aii);
}

QString Configurator::getYacappServerUrl() const
{
    return network.getYacappServerUrl();
}

