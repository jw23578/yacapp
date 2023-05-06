#ifndef YACAPP_H
#define YACAPP_H

#include <QObject>
#include "configmodels/parsedconfig.h"
#include "configmodels/globalprojectconfig.h"
#include "configmodels/appuserconfig.h"
#include "network/yacservernetwork.h"
#include "network/customservernetwork.h"
#include "network/asyncimageprovider.h"
#include "constants.h"
#include "helper.h"
#include <QJSValue>
#include "datamodels/templateddatamodel.h"
#include "datamodels/profilesmodel.h"
#include "localstorage/localstorage.h"
#include "datamodels/messagesmodel.h"
#include "firebase2qt.h"
#include <QTranslator>
#include <QTimer>
#include "yacAppAndServer/tablefields.h"
#include "dataobjects/appointmentobject.h"
#include "dataobjects/groupobject.h"
#include "dataobjects/spaceobject.h"
#include "dataobjects/multipurposeobject.h"
#include "dataobjects/worktimeobject.h"
#include "dataobjects/worktimemainobject.h"
#include "dataobjects/profileobject.h"
#include "dataobjects/spacerequestobject.h"
#include "dataobjects/newsobject.h"
#include "cppqmlobjects/cppqmlappandconfigurator.h"
#include "cppqmlobjects/thirdpartylogin.h"

#include "orm-mapper/orm2qjson.h"
#include "orm_implementions/t0021_right_group.h"
#include "orm_implementions/t0022_right_group2appuser.h"
#include "orm_implementions/t0023_right2rightgroup.h"
#include "orm_implementions/yacormfactory.h"

class Configurator;

class YACAPP : public QObject
{
    YACORMFactory factory;
    ORM2QJson orm2json;
    TableFields tableFields;
    QTimer timer;
    QTranslator translator;
    friend Configurator;
    friend AsyncImageProvider;
    Q_OBJECT
    CPPQMLAppAndConfigurator &cppQMLAppAndConfigurator;
    ThirdPartyLogin &thirdPartyLogin;
    Constants &constants;
    const Helper &helper;
    LocalStorage *localStorage;
    QString deviceToken;
    YACAPPPROPERTY(bool, secondStart, SecondStart, false);
    YACAPPPROPERTY(QString, appFolder, AppFolder, "");
    YACAPPPROPERTY(QDateTime, serverNow, ServerNow, QDateTime(QDate(1978, 1, 1), QTime(12, 0)));
    YACAPPPROPERTY(AppUserConfig*, appUserConfig, AppUserConfig, new AppUserConfig(0));
    YACAPPPROPERTY(ParsedConfig*, mainConfig, MainConfig, 0);
    YACAPPPROPERTY(QString, globalProjectConfigFilename, GlobalProjectConfigFilename, "");
    YACAPPPROPERTY(GlobalProjectConfig*, globalConfig, GlobalConfig, new GlobalProjectConfig(true));
    YACAPPPROPERTY(QStringList, knownFiles, KnownFiles, QStringList())
    YACAPPPROPERTY(QStringList, currentFetchedIds, CurrentFetchedId, QStringList());
    YACAPPPROPERTY(QStringList, moodModel, MoodModel, QStringList());
    QMap<QString, ParsedConfig*> fileName2ParsedConfig;
    void addKnownFile(QString const &filename);

    struct SFileToFetch
    {
        QString imageType;
        QString imageId;
        AsyncImageResponse *air;
        QString imageFilename;
    };
    std::vector<SFileToFetch> filesToFetch;
    std::mutex filesToFetchMutex;
    void addFileToFetch(const QString &imageType,
                        const QString &imageId,
                        AsyncImageResponse *air,
                        const QString &imageFilename);
    void fetchFiles();

    YACAPPPROPERTY(QStringList, knownMenueFiles, KnownMenueFiles, QStringList())
    QMap<QString, MenueConfig*> fileName2MenueConfig;
    void addKnownMenueFile(QString const &filename);

    MenueConfig emptyMenue;

    YACServerNetwork &network;
    AsyncImageProvider imageProvider;
    CustomServerNetwork &customServerNetwork;

    ProfilesModel searchProfilesModel;
    ProfilesModel knownProfilesModel;
    ProfilesModel selectedProfilesModel;

    MessagesModel messagesModel;

    TemplatedDataModel<AppointmentObject> appointmentsModel;

    TemplatedDataModel<YACBaseObject> rightGroupsModel;
    TemplatedDataModel<MultiPurposeObject> allRightsModel;

    TemplatedDataModel<SpaceObject> spacesModel;
    TemplatedDataModel<SpaceRequestObject> spaceRequestsModel;

    TemplatedDataModel<WorktimeMainObject> worktimeMainsModel;

    TemplatedDataModel<NewsObject> newsModel;

    void cleanUpKnownFile();

    void loadAppConfig();
    void saveAppConfig();

    template <class T>
    void appUserFetchORM(const QString ormName,
                         const std::map<QString, QString> &needles,
                         T &modelToAdd,
                         QJSValue successCallback,
                         QJSValue errorCallback)
    {
        network.appUserFetchORM(globalConfig()->projectID(),
                                appUserConfig()->loginEMail(),
                                appUserConfig()->loginToken(),
                                ormName, needles,
                                [this, ormName, &modelToAdd, successCallback](const QJsonDocument &jsonDoc) mutable
        {
            modelToAdd.clear();
            QJsonObject jsonObject(jsonDoc.object());
            QJsonArray array(jsonObject[ormName].toArray());
            for (int i(0); i < array.size(); ++i)
            {
                modelToAdd.append(orm2json.fromJson(array[i].toObject(), factory));
            }
            successCallback.call(QJSValueList());
        },
        [errorCallback](const QString &message) mutable
        {
            errorCallback.call(QJSValueList() << message);
        });
    }

public:
    Firebase2Qt firebase2qt;
    explicit YACAPP(QQmlApplicationEngine &engine
                    , CPPQMLAppAndConfigurator &cppQMLAppAndConfigurator
                    , ThirdPartyLogin &thirdPartyLogin
                    , Constants &constants
                    , const Helper &helper
                    , YACServerNetwork &network
                    , CustomServerNetwork &customServerNetwork
                    , QObject *parent = nullptr);
    void init();

    Q_INVOKABLE QString getProfileVisibleName(const QString &id);
    Q_INVOKABLE QString getProfileImageId(const QString &id);

    Q_INVOKABLE void logout();
    Q_INVOKABLE void leaveApp();
    Q_INVOKABLE void saveState();

    Q_INVOKABLE ParsedConfig *getConfig(QString const &filename);

    Q_INVOKABLE MenueConfig *getMenueConfig(QString const &filename);

    void loadAppAndInitialize(QString projectFilename);
    void saveAppToFile(QString projectFilename);

    Q_INVOKABLE void yacappServerGetAllAPPs(QJSValue successCallback,
                                            QJSValue  errorCallback);

    Q_INVOKABLE void yacappServerGetAPP(const QString &app_id,
                                        const QString &installation_code,
                                        const int current_installed_version,
                                        QJSValue successCallback,
                                        QJSValue errorCallback);

    Q_INVOKABLE void appUserRegister(const QString &loginEMail,
                                     const QString &password,
                                     QJSValue successCallback,
                                     QJSValue errorCallback);
    Q_INVOKABLE void appUserRequestVerifyToken(const QString &loginEMail,
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
                                           int userMood,
                                           int dayRating,
                                           QJSValue successCallback,
                                           QJSValue errorCallback);
    Q_INVOKABLE void appUserInsertWorktimeBeginEnd(const int worktimeType,
                                                   const QDateTime begin,
                                                   const QDateTime end,
                                                   QJSValue successCallback,
                                                   QJSValue errorCallback);
    Q_INVOKABLE void appUserFetchWorktimes(const QDateTime &since,
                                           const QDateTime &until,
                                           QJSValue successCallback,
                                           QJSValue errorCallback);

    Q_INVOKABLE void appUserDeleteWorktime(const QString &id,
                                           QJSValue successCallback,
                                           QJSValue errorCallback);


    Q_INVOKABLE void appUserUpdateProfile(const QString &fstname,
                                          const QString &surname,
                                          const QString &visible_name,
                                          const QString &profileFilename,
                                          const bool searching_exactly_allowed,
                                          const bool searching_fuzzy_allowed,
                                          const QString &password,
                                          QJSValue successCallback,
                                          QJSValue errorCallback);

    Q_INVOKABLE void appUserSearchProfiles(const QString &needle,
                                           const int limit,
                                           const int offset,
                                           QJSValue successCallback,
                                           QJSValue errorCallback);

    Q_INVOKABLE void appUserInsertAppointment(const QString &appointment_group_id,
                                              const QString &appointment_template_id,
                                              const QString &caption,
                                              const QDateTime &start_datetime,
                                              const QDateTime &end_datetime,
                                              const bool visible_for_everybody,
                                              QJSValue successCallback,
                                              QJSValue errorCallback);

    Q_INVOKABLE void appUserFetchAppointments(QJSValue successCallback,
                                              QJSValue errorCallback);
    Q_INVOKABLE void appUserDeleteAppointment(const QString &id,
                                              QJSValue successCallback,
                                              QJSValue errorCallback);

    Q_INVOKABLE void appUserFetchRightGroups(QJSValue successCallback,
                                             QJSValue errorCallback);
    Q_INVOKABLE void appUserInsertOrUpdateRightGroup(const QString &id,
                                                     const QString &name,
                                                     const bool automatic,
                                                     const QString access_code,
                                                     const bool request_allowed,
                                                     const bool visible_for_non_members,
                                                     QJSValue successCallback,
                                                     QJSValue errorCallback);
    Q_INVOKABLE void appUserDeleteRightGroup(const QString &id,
                                             QJSValue successCallback,
                                             QJSValue errorCallback);
    Q_INVOKABLE void appUserFetchRightGroupRights(const QString &id,
                                                  QJSValue successCallback,
                                                  QJSValue errorCallback);
    Q_INVOKABLE void appUserFetchRightGroupMember(const QString &right_group_id,
                                                  QJSValue successCallback,
                                                  QJSValue errorCallback);

    Q_INVOKABLE void appUserInsertOrUpdateRightGroup2AppUser(const QString &id,
                                                             const QString &right_group_id,
                                                             const QString &appuser_id,
                                                             const QDateTime &requested_datetime,
                                                             const QDateTime &approved_datetime,
                                                             const QString &approved_appuser_id,
                                                             const QDateTime &denied_datetime,
                                                             const QString &denied_appuser_id,
                                                             QJSValue successCallback,
                                                             QJSValue errorCallback);


    Q_INVOKABLE void appUserFetchSpaces(QJSValue successCallback,
                                        QJSValue errorCallback);
    Q_INVOKABLE void appUserInsertOrUpdateSpace(const QString &id,
                                                const QString &name,
                                                const QString &access_code,
                                                const bool automatic,
                                                const bool request_allowed,
                                                QJSValue successCallback,
                                                QJSValue errorCallback);
    Q_INVOKABLE void appUserDeleteSpace(const QString &id,
                                        QJSValue successCallback,
                                        QJSValue errorCallback);
    Q_INVOKABLE void appUserFetchSpace(const QString &id,
                                       QJSValue successCallback,
                                       QJSValue errorCallback);
    Q_INVOKABLE void appUserRequestSpaceAccess(const QString space_id,
                                               QJSValue successCallback,
                                               QJSValue errorCallback);


    Q_INVOKABLE void fetchMessageUpdates();

    Q_INVOKABLE void loadMessages(const QString &contactId);
    Q_INVOKABLE void sendMessage(const QString &profileId, const QString &content);
    Q_INVOKABLE void addProfileToKnownProfiles(const QString &id);
    Q_INVOKABLE void removeProfileFromKnownProfiles(const QString &id);

    Q_INVOKABLE void addProfileToSelectedProfiles(const QString &id);
    Q_INVOKABLE void removeFromSelectedProfiles(const QString &id);

    Q_INVOKABLE void switchLanguage(const QString &language);

    Q_INVOKABLE void goTakePhoto(bool squared, bool circled, QJSValue target);

    Q_INVOKABLE QString getNewProfileImageFilename();

    Q_INVOKABLE void fetchProfileAndUpsertKnownProfiles(const QString &profileId);

    Q_INVOKABLE void fetchMyProfile(QJSValue successCallback,
                                    QJSValue errorCallback);
signals:
    void takePhoto(bool squared, bool circled, QJSValue target);

private slots:
    void deviceTokenChanged(QString deviceToken);
    void newMessages();
    void timeout();
    void missingRight(int rightNumber);

};

#endif // YACAPP_H
