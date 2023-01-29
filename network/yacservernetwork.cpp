#include "yacservernetwork.h"
#include <QFileInfo>
#include <QDir>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

YACServerNetwork::YACServerNetwork(QNetworkAccessManager &manager
                                   , const Constants &constants):
    YACServerBaseNetwork(manager, constants)
{
}

void YACServerNetwork::yacappServerGetAllAPPs(CallbackFunction successCallback,
                                              CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        rr.successCallback(all);
    });
    const QUrlQuery query;
    yacappServerGet("/getAllAPPs",
                    query,
                    replyHandler,
                    successCallback,
                    errorCallback);
}

void YACServerNetwork::yacappServerGetAPP(const QString &app_id,
                                          const int current_installed_version,
                                          CallbackFunction successCallback,
                                          CallbackFunction errorCallback)
{
    auto replyHandler([this](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        if (message == "app version is up to date")
        {
            rr.successCallback(message);
            return;
        }
        if (message == "app found")
        {
            QString json_yacapp(object["json_yacapp"].toString());
            QString app_id(object["app_id"].toString());

            QByteArray yacpck_base64(object["yacpck_base64"].toString().toLatin1());
            QByteArray yacpck(QByteArray::fromBase64(yacpck_base64));
            QFile file(constants.getYacAppConfigPath() + app_id + ".yacapp");
            file.open(QIODevice::WriteOnly);
            file.write(json_yacapp.toLatin1());
            file.close();

            QByteArray uncompressedData(qUncompress(yacpck));
            int pos(0);
            while (pos < uncompressedData.size())
            {
                int nextPos(uncompressedData.indexOf('\0', pos));
                QString filename(uncompressedData.mid(pos, nextPos - pos));
                pos = nextPos + 1;
                nextPos = uncompressedData.indexOf('\0', pos);
                if (nextPos == -1)
                {
                    nextPos = uncompressedData.size();
                }
                QByteArray data(uncompressedData.mid(pos, nextPos - pos));

                QFile file(constants.getYacAppConfigPath() + filename);
                file.open(QIODevice::WriteOnly);
                file.write(data);
                file.close();
                pos = nextPos + 1;
            }
            rr.successCallback(message);
        }
        else
        {
            rr.errorCallback(message);
        }
    });
    QUrlQuery query;
    query.addQueryItem("app_id", app_id);
    query.addQueryItem("current_installed_version", QString::number(current_installed_version));
    yacappServerGet("/getAPP",
                    query,
                    replyHandler,
                    successCallback,
                    errorCallback);
}

void YACServerNetwork::yacappServerAppUserRegister(const QString &loginEMail,
                                                   const QString &password,
                                                   const QString &appId,
                                                   CallbackFunction successCallback,
                                                   CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        if (object["success"].toBool())
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
    obj["appId"] = appId;
    yacappServerPost("/registerAppUser",
                     obj,
                     replyHandler,
                     successCallback,
                     errorCallback);
}

void YACServerNetwork::yacappServerAppUserVerify(const QString &loginEMail,
                                                 const QString &verifyToken,
                                                 const QString &appId,
                                                 CallbackFunction successCallback,
                                                 CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        if (object["success"].toBool())
        {
            rr.successCallback(object["loginToken"].toString());
        }
        else
        {
            rr.errorCallback(message);
        }
    });

    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["verifyToken"] = verifyToken;
    obj["appId"] = appId;
    yacappServerPost("/verifyAppUser",
                     obj,
                     replyHandler,
                     successCallback,
                     errorCallback);
}

void YACServerNetwork::yacappServerAppUserLogin(const QString &loginEMail,
                                                const QString &password,
                                                const QString &appId,
                                                const QString &deviceToken,
                                                JSONCallbackFunction jsonSuccessCallback,
                                                CallbackFunction errorCallback)
{
    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["password"] = password;
    obj["appId"] = appId;
    obj["deviceToken"] = deviceToken;
    QMap<QByteArray, QByteArray> rawHeader;
    yacappServerPost("/loginAppUser",
                     obj,
                     defaultJSONReplyHandler,
                     rawHeader,
                     0,
                     jsonSuccessCallback,
                     errorCallback);
}

void YACServerNetwork::appUserRequestPasswordUpdate(const QString &loginEMail,
                                                    const QString &appId,
                                                    CallbackFunction successCallback,
                                                    CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        if (object["success"].toBool())
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
    obj["appId"] = appId;
    yacappServerPost("/requestUpdatePasswordAppUser",
                     obj,
                     replyHandler,
                     successCallback,
                     errorCallback);
}

void YACServerNetwork::appUserUpdatePassword(const QString &loginEMail,
                                             const QString &password,
                                             const QString &updatePasswordToken,
                                             const QString &appId,
                                             CallbackFunction successCallback,
                                             CallbackFunction errorCallback)
{
    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["password"] = password;
    obj["updatePasswordToken"] = updatePasswordToken;
    obj["appId"] = appId;
    yacappServerPost("/updatePasswordAppUser",
                     obj,
                     defaultReplyHandler,
                     successCallback,
                     errorCallback);

}

void YACServerNetwork::appUserGetWorktimeState(const QString &appId,
                                               const QString &loginEMail,
                                               const QString &loginToken,
                                               JSONCallbackFunction jsonSuccessCallback,
                                               CallbackFunction errorCallback)
{
    const QUrlQuery query;
    QMap<QByteArray, QByteArray> rawHeader;
    rawHeader["YACAPP-AppId"] = appId.toLatin1();
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();
    yacappServerGet("/getWorktimeState",
                    query,
                    defaultJSONReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);

}

void YACServerNetwork::appUserInsertWorktime(const QString &appId,
                                             const QString &loginEMail,
                                             const QString &loginToken,
                                             const int worktimeType,
                                             const QDateTime ts,
                                             JSONCallbackFunction jsonSuccessCallback,
                                             CallbackFunction errorCallback)
{
    QMap<QByteArray, QByteArray> rawHeader;
    rawHeader["YACAPP-AppId"] = appId.toLatin1();
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();

    QJsonObject obj;
    obj["worktimeType"] = worktimeType;
    obj["ts"] = ts.toTimeSpec(Qt::OffsetFromUTC).toString(Qt::ISODate);

    yacappServerPost("/insertWorktime",
                     obj,
                     defaultJSONReplyHandler,
                     rawHeader,
                     0,
                     jsonSuccessCallback,
                     errorCallback);


}

void YACServerNetwork::appUserSearchProfiles(const QString &appId,
                                             const QString &loginEMail,
                                             const QString &loginToken,
                                             const QString &needle,
                                             const int limit,
                                             const int offset,
                                             JSONCallbackFunction jsonSuccessCallback,
                                             CallbackFunction errorCallback)
{
    QUrlQuery query;
    query.addQueryItem("needle", needle);
    query.addQueryItem("limit", QString::number(limit));
    query.addQueryItem("offset", QString::number(offset));
    QMap<QByteArray, QByteArray> rawHeader;
    rawHeader["YACAPP-AppId"] = appId.toLatin1();
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();
    yacappServerGet("/appUserSearchProfiles",
                    query,
                    defaultJSONReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);
}

void YACServerNetwork::appUserUpdateDeviceToken(const QString &appId,
                                                const QString &loginEMail,
                                                const QString &loginToken,
                                                const QString &deviceToken,
                                                CallbackFunction successCallback,
                                                CallbackFunction errorCallback)
{
    QMap<QByteArray, QByteArray> rawHeader;
    rawHeader["YACAPP-AppId"] = appId.toLatin1();
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();

    QJsonObject obj;
    obj["deviceToken"] = deviceToken;
    yacappServerPost("/updateDeviceToken",
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     successCallback,
                     errorCallback);
}

void YACServerNetwork::appUserStoreMessage(const QString &appId,
                                           const QString &loginEMail,
                                           const QString &loginToken,
                                           const QString &id,
                                           const QString &to_id,
                                           const QString &content_base64,
                                           CallbackFunction successCallback,
                                           CallbackFunction errorCallback)
{
    QMap<QByteArray, QByteArray> rawHeader;
    rawHeader["YACAPP-AppId"] = appId.toLatin1();
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();

    QJsonObject obj;
    obj["id"] = id;
    obj["to_id"] = to_id;
    obj["content_base64"] = content_base64;

    yacappServerPost("/storeMessage",
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     successCallback,
                     0,
                     errorCallback);
}

void YACServerNetwork::appUserFetchMessageUpdates(const QString &appId,
                                                  const QString &loginEMail,
                                                  const QString &loginToken,
                                                  const QDateTime &updatesSince,
                                                  JSONCallbackFunction jsonSuccessCallback,
                                                  CallbackFunction errorCallback)
{
    QMap<QByteArray, QByteArray> rawHeader;
    rawHeader["YACAPP-AppId"] = appId.toLatin1();
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();

    QUrlQuery query;
    QString help(updatesSince.toString(Qt::ISODate));
    query.addQueryItem("updatesSinceISO", updatesSince.toString(Qt::ISODate));

    yacappServerGet("/fetchMessageUpdates",
                    query,
                    defaultJSONReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);
}

void YACServerNetwork::appUserFetchProfile(const QString &appId,
                                           const QString &loginEMail,
                                           const QString &loginToken,
                                           const QString &profileId,
                                           JSONCallbackFunction jsonSuccessCallback,
                                           CallbackFunction errorCallback)
{
    QUrlQuery query;
    query.addQueryItem("profileId", profileId);

    yacappServerAppUserGetJSONReply("/fetchProfile",
                                    query,
                                    appId,
                                    loginEMail,
                                    loginToken,
                                    jsonSuccessCallback,
                                    errorCallback);
    }
