#ifndef YACNETWORK_H
#define YACNETWORK_H

#include "yacserverbasenetwork.h"

class YACNetwork: public YACServerBaseNetwork
{
    Q_OBJECT
public:
    YACNetwork(QNetworkAccessManager &manager
               , const Constants &constants);
    void yacappServerRegisterUser(QString loginEMail,
                                  QString password,
                                  CallbackFunction successCallback,
                                  CallbackFunction  errorCallback);
    void yacappServerVerifyUser(QString loginEMail,
                                QString verifyToken,
                                CallbackFunction successCallback,
                                CallbackFunction  errorCallback);
    void yacappServerLoginUser(QString loginEMail,
                               QString password,
                               CallbackFunction successCallback,
                               CallbackFunction  errorCallback);
    void yacappServerUserLoggedIn(QString loginEMail,
                                  QString verifyToken,
                                  CallbackFunction successCallback,
                                  CallbackFunction  errorCallback);

    void yacappServerUploadApp(const QString &loginEMail,
                               const QString &loginToken,
                               const QString &app_id,
                               const QString &app_name,
                               const int app_version,
                               const QString &app_logo_url,
                               const QString &app_color_name,
                               const QString &json_yacapp,
                               const QString &yacpck_base64,
                               CallbackFunction successCallback,
                               CallbackFunction  errorCallback);

    void yacappServerGetAllAPPs(CallbackFunction successCallback,
                                CallbackFunction  errorCallback);

    void yacappServerGetAPP(const QString &app_id,
                            const int current_installed_version,
                            CallbackFunction successCallback,
                            CallbackFunction  errorCallback);
};

#endif // YACNETWORK_H
