#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QObject>
#include "yacappmacros.h"
#include "projectdata.h"
#include <QMap>
#include "network/yacextservernetwork.h"
#include <QJSValue>
#include "yacapp.h"

class Configurator : public QObject
{
    Q_OBJECT
    QString configFilename;

    YACAPP &yacApp;
    YACExtServerNetwork &network;

    YACAPPPROPERTY(QString, lastProjectName, LastProjectName, "");
    YACAPPPROPERTY(QString, lastProjectFilename, LastProjectFilename, "");

    QMap<QString, ProjectData*> deployConfigs;

    YACAPPPROPERTY(ProjectData*, activeProjectData, ActiveProjectData, new ProjectData);

    void sftpUpload(QString host, QString user, QString password, QString targetFilename, QString sourceFilename);

public:
    explicit Configurator(YACAPP &yacApp
                          , YACExtServerNetwork &network
                          , QObject *parent = nullptr);

    Q_INVOKABLE void save();

    Q_INVOKABLE void deploy(QString projectID, QString host, QString user, QString password, QString www_basedirectory);

    Q_INVOKABLE void defaultDeploy(const QString &globalProjectConfigFilename, QString host, QString user, QString password);

    Q_INVOKABLE void deploy(QString globalProjectConfigFilename,
                                          QJSValue goodCallback,
                                          QJSValue badCallback);

    Q_INVOKABLE void deployToYACAPPServer(QString globalProjectConfigFilename,
                                          QJSValue goodCallback,
                                          QJSValue badCallback);

    Q_INVOKABLE void setProjectData(const QString &projectID);

    Q_INVOKABLE void yacserverLogin(const QString &loginEMail,
                                    const QString &password,
                                    const QString &projectID,
                                    QJSValue goodCallback,
                                    QJSValue badCallback);

    Q_INVOKABLE void yacserverUserLoggedIn(const QString &loginEMail, const QString &loginToken, const QString &projectID);
    Q_INVOKABLE void yacserverRegister(const QString &loginEMail
                                       , const QString &password
                                       , QJSValue goodCallback
                                       , QJSValue badCallback);

    Q_INVOKABLE void yacserverVerify(const QString &loginEMail
                                     , const QString &verifyToken
                                     , QJSValue goodCallback
                                     , QJSValue badCallback);

    Q_INVOKABLE bool isFolderEmpty(const QString &folder);

    Q_INVOKABLE void createNewProject(const QString &projectName,
                                      const QString &projectFolder);

    Q_INVOKABLE void addFormFile(QString fileUrl);

signals:

    void addFileSignal(const QJSValue okCallback);
};

#endif // CONFIGURATOR_H
