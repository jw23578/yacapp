#ifndef YACEXTSERVERNETWORK_H
#define YACEXTSERVERNETWORK_H

#include "yacservernetwork.h"

class YACExtServerNetwork : public YACServerNetwork
{
public:
    YACExtServerNetwork(QNetworkAccessManager &manager
                        , Constants &constants);

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
                               const bool is_template_app,
                               const QString &json_yacapp,
                               const QString &yacpck_base64,
                               CallbackFunction successCallback,
                               CallbackFunction  errorCallback);

};

#endif // YACEXTSERVERNETWORK_H
