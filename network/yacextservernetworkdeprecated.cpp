#include "yacextservernetworkdeprecated.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include "orm_implementions/t0001_apps.h"
#include "orm-mapper/orm2qjson.h"

YACExtServerNetworkDeprecated::YACExtServerNetworkDeprecated(QNetworkAccessManager &manager
                                                             , Constants &constants)
    :YACServerNetwork(manager
                       , constants)
{

}
/*
void YACExtServerNetworkDeprecated::yacappServerVerifyUser(QString loginEMail, QString verifyToken, CallbackFunction successCallback, CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply,
                      QByteArray &allData,
                      SRunningRequest &rr)
    {
        Q_UNUSED(finishedReply);
        QJsonDocument loginReplyDoc(QJsonDocument::fromJson(allData));
        QJsonObject object(loginReplyDoc.object());
        QString message(object["message"].toString());
        if (message == "user logged out")
        {
            rr.successCallback("");
        }
        else
        {
            rr.errorCallback(message);
        }
    });

    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["verifyToken"] = verifyToken;
    yacappServerPost("/verifyUser",
                     obj,
                     replyHandler,
                     successCallback,
                     errorCallback);
}*/

void YACExtServerNetworkDeprecated::yacappServerUploadApp(const QString &loginEMail,
                                                          const QString &loginToken,
                                                          const QString &appId,
                                                          t0001_apps &theApp,
                                                          const QString &installation_code,
                                                          CallbackFunction successCallback,
                                                          CallbackFunction errorCallback)
{    
    RequestData rd;
    rd.method = methodNames.uploadApp;
    ORM2QJson orm2json;
    orm2json.toJson(theApp, rd.object);
    rd.object["installation_code"] = installation_code;
    rd.handlerFunction = defaultReplyHandler;
    rd.successCallback = successCallback;
    rd.errorCallback = errorCallback;
    rd.addAppId(appId);
    rd.addLoginEMail(loginEMail);
    rd.addLoginToken(loginToken);
    post(rd);
}

