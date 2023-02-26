#ifndef YACSERVERNETWORK_H
#define YACSERVERNETWORK_H

#include "yacserverbasenetwork.h"
#include "yacAppAndServer/yacappservermethodnames.h"

class YACServerNetwork: public YACServerBaseNetwork
{
    Q_OBJECT
    YACAPPServerMethodNames methodNames;
public:
    YACServerNetwork(QNetworkAccessManager &manager
                     , Constants &constants);

    void yacappServerGetAllAPPs(CallbackFunction successCallback,
                                CallbackFunction  errorCallback);

    void yacappServerGetAPP(const QString &app_id,
                            const int current_installed_version,
                            CallbackFunction successCallback,
                            CallbackFunction  errorCallback);

    void yacappServerAppUserRegister(const QString &loginEMail,
                                     const QString &password,
                                     const QString &appId,
                                     CallbackFunction successCallback,
                                     CallbackFunction  errorCallback);

    void yacappServerAppUserRequestVerifyToken(const QString &loginEMail,
                                               const QString &appId,
                                               CallbackFunction successCallback,
                                               CallbackFunction  errorCallback);

    void yacappServerAppUserVerify(const QString &loginEMail,
                                   const QString &verifyToken,
                                   const QString &appId,
                                   JSONCallbackFunction jsonSuccessCallback,
                                   CallbackFunction  errorCallback);

    void yacappServerAppUserLogin(const QString &loginEMail,
                                  const QString &password,
                                  const QString &appId,
                                  const QString &deviceToken,
                                  JSONCallbackFunction successCallback,
                                  CallbackFunction  errorCallback);

    void appUserRequestPasswordUpdate(const QString &loginEMail,
                                      const QString &appId,
                                      CallbackFunction successCallback,
                                      CallbackFunction  errorCallback);

    void appUserUpdatePassword(const QString &loginEMail,
                               const QString &password,
                               const QString &updatePasswordToken,
                               const QString &appId,
                               CallbackFunction successCallback,
                               CallbackFunction errorCallback);

    void appUserGetWorktimeState(const QString &appId,
                                 const QString &loginEMail,
                                 const QString &loginToken,
                                 JSONCallbackFunction successCallback,
                                 CallbackFunction  errorCallback);
    void appUserInsertWorktime(const QString &appId,
                               const QString &loginEMail,
                               const QString &loginToken,
                               const int workTimeType,
                               const int userMood,
                               const int dayRating,
                               const QDateTime ts,
                               JSONCallbackFunction jsonSuccessCallback,
                               CallbackFunction  errorCallback);

    void appUserSearchProfiles(const QString &appId,
                               const QString &loginEMail,
                               const QString &loginToken,
                               const QString &needle,
                               const int limit,
                               const int offset,
                               JSONCallbackFunction jsonSuccessCallback,
                               CallbackFunction  errorCallback);

    void appUserUpdateDeviceToken(const QString &appId,
                                  const QString &loginEMail,
                                  const QString &loginToken,
                                  const QString &deviceToken,
                                  CallbackFunction successCallback,
                                  CallbackFunction errorCallback);

    void appUserStoreMessage(const QString &appId,
                             const QString &loginEMail,
                             const QString &loginToken,
                             const QString &id,
                             const QString &to_id,
                             const QString &content_base64,
                             CallbackFunction successCallback,
                             CallbackFunction errorCallback);

    void appUserUpdateProfile(const QString &appId,
                              const QString &loginEMail,
                              const QString &loginToken,
                              const QString &fstname,
                              const QString &surname,
                              const QString &visible_name,
                              const QString &profileFilename,
                              const bool searching_exactly_allowed,
                              const bool searching_fuzzy_allowed,
                              CallbackFunction successCallback,
                              CallbackFunction errorCallback);

    void appUserFetchMessageUpdates(const QString &appId,
                                    const QString &loginEMail,
                                    const QString &loginToken,
                                    const QDateTime &updatesSince,
                                    JSONCallbackFunction jsonSuccessCallback,
                                    CallbackFunction  errorCallback);

    void appUserFetchProfile(const QString &appId,
                             const QString &loginEMail,
                             const QString &loginToken,
                             const QString &profileId,
                             JSONCallbackFunction jsonSuccessCallback,
                             CallbackFunction  errorCallback);

    void appUserFetchMyProfile(const QString &appId,
                               const QString &loginEMail,
                               const QString &loginToken,
                               JSONCallbackFunction jsonSuccessCallback,
                               CallbackFunction  errorCallback);

    void appUserFetchImage(const QString &appId,
                           const QString &loginEMail,
                           const QString &loginToken,
                           const QString &imageType,
                           const QString &imageId,
                           JSONCallbackFunction jsonSuccessCallback,
                           CallbackFunction errorCallback);

    void appUserInsertAppointment(const QString &appId,
                                  const QString &loginEMail,
                                  const QString &loginToken,
                                  const QString &appointment_group_id,
                                  const QString &appointment_template_id,
                                  const QString &caption,
                                  const QDateTime &start_datetime,
                                  const QDateTime &end_datetime,
                                  const bool visible_for_everybody,
                                  JSONCallbackFunction jsonSuccessCallback,
                                  CallbackFunction errorCallback);

    void appUserFetchAppointments(const QString &appId,
                                  const QString &loginEMail,
                                  const QString &loginToken,
                                  JSONCallbackFunction jsonSuccessCallback,
                                  CallbackFunction errorCallback);

};

#endif // YACSERVERNETWORK_H
