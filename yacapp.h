#ifndef YACAPP_H
#define YACAPP_H

#include <QObject>
#include "configmodels/parsedconfig.h"
#include "configmodels/globalprojectconfig.h"
#include "configmodels/appuserconfig.h"
#include "network/yacservernetwork.h"
#include "network/customservernetwork.h"
#include "constants.h"
#include "helper.h"
#include <QJSValue>
#include "datamodels/templateddatamodel.h"
#include "dataobjects/profileobject.h"
#include "datamodels/profilesmodel.h"
#include "localstorage/localstorage.h"
#include "datamodels/messagesmodel.h"

class Configurator;

class YACAPP : public QObject
{
    friend Configurator;
    Q_OBJECT
    const Constants &constants;
    const Helper &helper;
    LocalStorage &localStorage;
    YACAPPPROPERTY(QString, appFolder, AppFolder, "");
    YACAPPPROPERTY(QString, loginToken, LoginToken, "");
    YACAPPPROPERTY(QDateTime, serverNow, ServerNow, QDateTime(QDate(1978, 1, 1), QTime(12, 0)));
    YACAPPPROPERTY(AppUserConfig*, appUserConfig, AppUserConfig, new AppUserConfig(0));
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

    ProfilesModel searchProfilesModel;
    ProfilesModel knownProfilesModel;

    MessagesModel messagesModel;


    void cleanUpKnownFile();

public:
    explicit YACAPP(QQmlApplicationEngine &engine
                    , const Constants &constants
                    , const Helper &helper
                    , LocalStorage &localStorage
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

    Q_INVOKABLE void appUserLogout();


    Q_INVOKABLE void appUserRequestPasswordUpdate(const QString &loginEMail,
                                                  QJSValue successCallback,
                                                  QJSValue errorCallback);
    Q_INVOKABLE void appUserUpdatePassword(const QString &loginEMail,
                                           const QString &password,
                                           const QString &updatePasswordToken,
                                           QJSValue successCallback,
                                           QJSValue errorCallback);
    Q_INVOKABLE void appUserGetWorktimeState(QJSValue successCallback,
                                             QJSValue errorCallback);
    Q_INVOKABLE void appUserInsertWorktime(int worktimeType,
                                           QJSValue successCallback,
                                           QJSValue errorCallback);

    Q_INVOKABLE void appUserSearchProfiles(const QString &needle,
                                           const int limit,
                                           const int offset,
                                           QJSValue successCallback,
                                           QJSValue errorCallback);

    Q_INVOKABLE void fetchMessageUpdates();

    Q_INVOKABLE void loadMessages(const QString &contactId);
    Q_INVOKABLE void sendMessage(const QString &profileId, const QString &content);
    Q_INVOKABLE void addProfileToKnownProfiles(const QString &id);
    Q_INVOKABLE void removeProfileFromKnownProfiles(const QString &id);
signals:

    void badMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);
    void goodMessage(const QString &message, QJSValue itemToFocus, QJSValue okCallback);
    void yesNoQuestion(const QString &question, QJSValue itemToFocus, QJSValue yesCallback, QJSValue noCallback);
};

#endif // YACAPP_H
