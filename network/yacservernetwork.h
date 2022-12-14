#ifndef YACSERVERNETWORK_H
#define YACSERVERNETWORK_H

#include "yacserverbasenetwork.h"

class YACServerNetwork: public YACServerBaseNetwork
{
    Q_OBJECT
public:
    YACServerNetwork(QNetworkAccessManager &manager
                     , const Constants &constants);

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

    void yacappServerAppUserVerify(const QString &loginEMail,
                                   const QString &verifyToken,
                                   const QString &appId,
                                   CallbackFunction successCallback,
                                   CallbackFunction  errorCallback);

    void yacappServerAppUserLogin(const QString &loginEMail,
                                  const QString &password,
                                  const QString &appId,
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

    void appUserStoreMessage(const QString &appId,
                             const QString &loginEMail,
                             const QString &loginToken,
                             const QString &id,
                             const QString &to_id,
                             const QString &content_base64,
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

};

#endif // YACSERVERNETWORK_H
