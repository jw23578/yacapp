#include "yacextservernetwork.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include "orm_implementions/t0002_apps.h"
#include "orm-mapper/orm2qjson.h"

YACExtServerNetwork::YACExtServerNetwork(QNetworkAccessManager &manager
                                         , Constants &constants)
    :YACServerNetwork(manager
                      , constants)
{

}

void YACExtServerNetwork::yacappServerRegisterUser(QString loginEMail,
                                                   QString password,
                                                   CallbackFunction successCallback,
                                                   CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply,
                      QByteArray &allData,
                      SRunningRequest &rr)
    {
        QJsonDocument replyDoc(QJsonDocument::fromJson(allData));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        if (message == "user registered, please verify")
        {
            rr.successCallback(message);
        }
        else
        {
            rr.errorCallback(message);
        }
    });

    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["password"] = password;
    yacappServerPost("/registerUser",
                     obj,
                     replyHandler,
                     successCallback,
                     errorCallback);
}

void YACExtServerNetwork::yacappServerVerifyUser(QString loginEMail, QString verifyToken, CallbackFunction successCallback, CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply,
                      QByteArray &allData,
                      SRunningRequest &rr)
    {
        QJsonDocument loginReplyDoc(QJsonDocument::fromJson(allData));
        QJsonObject object(loginReplyDoc.object());
        if (object["loginToken"].toString().size())
        {
            rr.successCallback(object["loginToken"].toString());
        }
        else
        {
            rr.errorCallback(object["message"].toString());
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
}

void YACExtServerNetwork::yacappServerLoginUser(QString loginEMail, QString password, CallbackFunction successCallback, CallbackFunction errorCallback)
{
    auto replyHandler = [](QNetworkReply *finishedReply,
            QByteArray &allData,
            SRunningRequest &rr)
    {
        QJsonDocument loginReplyDoc(QJsonDocument::fromJson(allData));
        QJsonObject object(loginReplyDoc.object());
        if (object["loginToken"].toString().size())
        {
            rr.successCallback(object["loginToken"].toString());
        }
        else
        {
            rr.errorCallback(object["message"].toString());
        }
    };


    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["password"] = password;
    yacappServerPost("/loginUser",
                     obj,
                     replyHandler,
                     successCallback,
                     errorCallback);
}

void YACExtServerNetwork::yacappServerUserLoggedIn(QString loginEMail, QString verifyToken, CallbackFunction successCallback, CallbackFunction errorCallback)
{
    QUrlQuery query;
    query.addQueryItem("loginEMail", loginEMail);
    query.addQueryItem("verifyToken", verifyToken);
    yacappServerGet("/userLoggedIn", query,
                    defaultReplyHandler,
                    successCallback,
                    errorCallback);
}

void YACExtServerNetwork::yacappServerUploadApp(const QString &loginEMail,
                                                const QString &loginToken,
                                                const QString &app_id,
                                                const QString &app_name,
                                                const int app_version,
                                                const QString &app_logo_url,
                                                const QString &app_info_url,
                                                const QString &search_code,
                                                const QString &installation_code,
                                                const QString &app_color_name,
                                                const bool is_template_app,
                                                const QString &json_yacapp,
                                                const QString &yacpck_base64,
                                                CallbackFunction successCallback,
                                                CallbackFunction errorCallback)
{
    t0002_apps app;
    app.setid(app_id);
    app.setapp_name(app_name);
    app.setapp_version(app_version);
    app.setapp_logo_url(app_logo_url);
    app.setapp_info_url(app_info_url);
    app.setsearch_code(search_code);
    app.setapp_color_name(app_color_name);
    app.setis_template_app(is_template_app);
    app.setyacpck_base64(0);
    app.settransfer_yacpck_base64(yacpck_base64);
    app.setjson_yacapp(json_yacapp);


    QJsonObject obj;
    ORM2QJson orm2json;
    orm2json.toJson(app, obj);
    obj["installation_code"] = installation_code;

    QMap<QByteArray, QByteArray> rawHeader;
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();
    yacappServerPost("/uploadApp",
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     successCallback,
                     errorCallback);
}

