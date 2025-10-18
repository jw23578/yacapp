#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QObject>
#include "yacappmacros.h"
#include "projectdata.h"
#include <QMap>
#include "network/yacservernetwork.h"
#include <QJSValue>
#include "yacapp.h"
#include "configuratormodels/recentproject.h"
#include "cppqmlobjects/cppqmlappandconfigurator.h"
#include "helper.h"


class Configurator : public QObject
{
    Q_OBJECT
    QString configFilename;

    YACAPP &yacApp;
    Helper &helper;
    CPPQMLAppAndConfigurator &cppQMLAppAndConfigurator;
    YACServerNetwork &network;
    const QString yacAppConfiguratorConfig;

    const QString theCreatorAppAppId = "00000000-0000-0000-0000-000000000001";

    YACAPPPROPERTY(QString, lastProjectName, LastProjectName, "");
    YACAPPPROPERTY(QString, lastProjectFilename, LastProjectFilename, "");
    YACAPPPROPERTY(QString, lastProjectLogoUrl, LastProjectLogoUrl, "");
    YACAPPPROPERTY(QString, deployUser, DeployUser, "");
    YACAPPPROPERTY(QString, yacappServerLoginToken, YacappServerLoginToken, "");

    YACAPPPROPERTY(int, recentItemCount, RecentItemCount, 0);
    YACOBJECTLISTPROPERTY(RecentProject, recentProject, RecentProject, Configurator)

    QMap<QString, ProjectData*> deployConfigs;

    YACAPPPROPERTY(ProjectData*, activeProjectData, ActiveProjectData, new ProjectData);

public:
    explicit Configurator(YACAPP &yacApp
                          , Helper &helper
                          , CPPQMLAppAndConfigurator &cppQMLAppAndConfigurator
                          , YACServerNetwork &network
                          , QObject *parent = nullptr);

    Q_INVOKABLE void save();

    Q_INVOKABLE void deploy(QJSValue goodCallback,
                            QJSValue badCallback);

    Q_INVOKABLE void setProjectData(const QString &projectID);

    Q_INVOKABLE void yacserverLogin(const QString &loginEMail,
                                    const QString &password,
                                    QJSValue goodCallback,
                                    QJSValue badCallback);
    Q_INVOKABLE void yacserverLogout(const QString &loginEMail,
                                     const QString &loginToken,
                                     QJSValue goodCallback,
                                     QJSValue badCallback);

    Q_INVOKABLE void yacserverUserLoggedIn(const QString &loginEMail,
                                           const QString &loginToken,
                                           const QString &projectID);
    Q_INVOKABLE void yacserverRegister(const QString &loginEMail
                                       , const QString &password
                                       , QJSValue goodCallback
                                       , QJSValue badCallback);
    Q_INVOKABLE void requestVerifyTokenCreator(const QString &loginEMail
                                               , QJSValue goodCallback
                                               , QJSValue badCallback);

    Q_INVOKABLE void verifyCreator(const QString &loginEMail
                                   , const QString &verifyToken
                                   , QJSValue goodCallback
                                   , QJSValue badCallback);

    Q_INVOKABLE void createNewProject(const QString &projectName
                                      , const QString &projectFolder
                                      , QJSValue okCallback
                                      , QJSValue projectNameEdit
                                      , QJSValue projectFolderEdit);
    Q_INVOKABLE void loadProjectFromFile(const QString &projectFilename);
    Q_INVOKABLE void saveCurrentProject();



    Q_INVOKABLE void addFormFile(QString fileUrl);
    Q_INVOKABLE void addMenueFile(QString fileUrl);

    Q_INVOKABLE void addImageFile(QString fileUrl);

    Q_INVOKABLE QString getYacappServerUrl() const;

signals:

    void takePhoto(bool squared, bool circled, QJSValue target);
    void addFileSignal(const QJSValue okCallback);
    void addImageSignal(const QJSValue okCallback);
};

#endif // CONFIGURATOR_H
