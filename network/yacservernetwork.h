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
                                     CallbackFunction successCallback,
                                     CallbackFunction  errorCallback);

    void yacappServerAppUserLogin(const QString &loginEMail,
                                  const QString &password,
                                  const QString &appId,
                                  CallbackFunction successCallback,
                                  CallbackFunction  errorCallback);};

#endif // YACSERVERNETWORK_H
