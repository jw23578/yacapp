#ifndef YACEXTSERVERNETWORKDEPRECATED_H
#define YACEXTSERVERNETWORKDEPRECATED_H

#include "yacservernetwork.h"
#include "orm_implementions/t0001_apps.h"

class YACExtServerNetworkDeprecated : public YACServerNetwork
{
public:
    YACExtServerNetworkDeprecated(QNetworkAccessManager &manager
                        , Constants &constants);

    void yacappServerUploadApp(const QString &loginEMail,
                               const QString &loginToken,
                               const QString &appId,
                               t0001_apps &theApp,
                               const QString &installation_code,
                               CallbackFunction successCallback,
                               CallbackFunction errorCallback);

};

#endif // YACEXTSERVERNETWORKDEPRECATED_H
