#include "yacextservernetwork.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>

YACExtServerNetwork::YACExtServerNetwork(QNetworkAccessManager &manager
                                         , const Constants &constants)
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
                      SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        qDebug() << __FILE__ << ": " << __LINE__ << all;
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
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
    auto replyHandler([](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        qDebug() << __FILE__ << ": " << __LINE__ << all;
        QJsonDocument loginReplyDoc(QJsonDocument::fromJson(all));
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
    auto replyHandler = [](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        QJsonDocument loginReplyDoc(QJsonDocument::fromJson(all));
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
    auto replyHander = [](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        Q_UNUSED(rr);
        qDebug() << __FILE__ << ": " << __LINE__ << finishedReply->readAll();
    };

    QUrlQuery query;
    query.addQueryItem("loginEMail", loginEMail);
    query.addQueryItem("verifyToken", verifyToken);
    yacappServerGet("/userLoggedIn", query,
                    replyHander,
                    successCallback,
                    errorCallback);
}

void YACExtServerNetwork::yacappServerUploadApp(const QString &loginEMail,
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
                                                CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        if (message == "new yacApp stored")
        {
            rr.successCallback(message);
        }
        else
        {
            rr.errorCallback(message);
        }
    });

    QJsonObject obj;
    obj["app_id"] = app_id;
    obj["app_name"] = app_name;
    obj["app_version"] = app_version;
    obj["app_logo_url"] = app_logo_url;
    obj["app_color_name"] = app_color_name;
    obj["is_template_app"] = is_template_app;
    obj["json_yacapp"] = json_yacapp;
    obj["yacpck_base64"] = yacpck_base64;
    QMap<QByteArray, QByteArray> rawHeader;
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();
    yacappServerPost("/uploadApp",
                     obj,
                     replyHandler,
                     rawHeader,
                     successCallback,
                     errorCallback);
}

