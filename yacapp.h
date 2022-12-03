#ifndef YACAPP_H
#define YACAPP_H

#include <QObject>
#include "configmodels/parsedconfig.h"
#include "configmodels/globalprojectconfig.h"
#include "network/yacservernetwork.h"
#include "network/customservernetwork.h"
#include "constants.h"
#include "helper.h"
#include <QJSValue>

class Configurator;

class YACAPP : public QObject
{
    friend Configurator;
    Q_OBJECT
    const Constants &constants;
    const Helper &helper;
    YACAPPPROPERTY(QString, appFolder, AppFolder, "");
    YACAPPPROPERTY(QString, loginToken, LoginToken, "");
    YACAPPPROPERTY(ParsedConfig*, mainConfig, MainConfig, 0);
    YACAPPPROPERTY(QString, globalProjectConfigFilename, GlobalProjectConfigFilename, "");
    YACAPPPROPERTY(GlobalProjectConfig*, globalConfig, GlobalConfig, new GlobalProjectConfig(true));
    YACAPPPROPERTY(QStringList, knownFiles, KnownFiles, QStringList())
    QMap<QString, ParsedConfig*> fileName2ParsedConfig;
    void addKnownFile(QString const &filename);

    YACAPPPROPERTY(QStringList, knownMenueFiles, KnownMenueFiles, QStringList())
    QMap<QString, MenueConfig*> fileName2MenueConfig;
    void addKnownMenueFile(QString const &filename);

    MenueConfig emptyMenue;

    YACServerNetwork &network;
    CustomServerNetwork &customServerNetwork;

    void cleanUpKnownFile();

public:
    explicit YACAPP(const Constants &constants
                    , const Helper &helper
                    , YACServerNetwork &network
                    , CustomServerNetwork &customServerNetwork
                    , QObject *parent = nullptr);
    void init(QString projectFilename);

    Q_INVOKABLE void logout();
    Q_INVOKABLE void leaveApp();
    Q_INVOKABLE void saveState();

    Q_INVOKABLE ParsedConfig *getConfig(QString const &filename);

    Q_INVOKABLE MenueConfig *getMenueConfig(QString const &filename);

    Q_INVOKABLE void loadNewProject(QString const &projectFilename);

    Q_INVOKABLE void saveCurrentProject();

    Q_INVOKABLE void downloadApp(QString url,
                                 QString projectID,
                                 QJSValue successCallback,
                                 QJSValue errorCallback);

    Q_INVOKABLE void yacappServerGetAllAPPs(QJSValue successCallback,
                                            QJSValue  errorCallback);

    Q_INVOKABLE void yacappServerGetAPP(const QString &app_id,
                                        const int current_installed_version,
                                        QJSValue successCallback,
                                        QJSValue errorCallback);

    Q_INVOKABLE void appUserRegister(const QString &loginEMail,
                                     const QString &password,
                                     QJSValue successCallback,
                                     QJSValue errorCallback);
    Q_INVOKABLE void appUserVerify(const QString &loginEMail,
                                   const QString &verifyToken,
                                   QJSValue successCallback,
                                   QJSValue errorCallback);
    Q_INVOKABLE void appUserLogin(const QString &loginEMail,
                                  const QString &password,
                                  QJSValue successCallback,
                                  QJSValue errorCallback);


    Q_INVOKABLE void appUserRequestPasswordUpdate(const QString &loginEMail,
                                                  QJSValue successCallback,
                                                  QJSValue errorCallback);
    Q_INVOKABLE void appUserUpdatePassword(const QString &loginEMail,
                                           const QString &password,
                                           const QString &updatePasswordToken,
                                           QJSValue successCallback,
                                           QJSValue errorCallback);
signals:

    void badMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);
    void goodMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);
    void yesNoQuestion(const QString &question, QJSValue itemToFocus, QJSValue yesCallback, QJSValue noCallback);
};

#endif // YACAPP_H
