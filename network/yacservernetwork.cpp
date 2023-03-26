#include "yacservernetwork.h"
#include <QFileInfo>
#include <QDir>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

YACServerNetwork::YACServerNetwork(QNetworkAccessManager &manager
                                   , Constants &constants):
    YACServerBaseNetwork(manager, constants)
{
}

void YACServerNetwork::yacappServerGetAllAPPs(CallbackFunction successCallback,
                                              CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply,
                      QByteArray &allData,
                      SRunningRequest &rr)
    {
        rr.successCallback(allData);
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
    auto replyHandler([this](QNetworkReply *finishedReply,
                      QByteArray &allData,
                      SRunningRequest &rr)
    {
        QJsonDocument replyDoc(QJsonDocument::fromJson(allData));
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
            QFile file(constants.getYacAppConfigPath(app_id) + app_id + ".yacapp");
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

                QFile file(constants.getYacAppConfigPath(app_id) + filename);
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
    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["password"] = password;
    obj["appId"] = appId;
    yacappServerPost("/registerAppUser",
                     obj,
                     defaultReplyHandler,
                     successCallback,
                     errorCallback);
}

void YACServerNetwork::yacappServerAppUserRequestVerifyToken(const QString &loginEMail,
                                                             const QString &appId,
                                                             CallbackFunction successCallback,
                                                             CallbackFunction errorCallback)
{
    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["appId"] = appId;
    yacappServerPost("/requestVerifyToken",
                     obj,
                     defaultReplyHandler,
                     successCallback,
                     errorCallback);
}

void YACServerNetwork::yacappServerAppUserVerify(const QString &loginEMail,
                                                 const QString &verifyToken,
                                                 const QString &appId,
                                                 JSONCallbackFunction jsonSuccessCallback,
                                                 CallbackFunction errorCallback)
{
    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["verifyToken"] = verifyToken;
    obj["appId"] = appId;
    QMap<QByteArray, QByteArray> rawHeader;
    yacappServerPost("/verifyAppUser",
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     0,
                     jsonSuccessCallback,
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
                     defaultReplyHandler,
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
    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["appId"] = appId;
    yacappServerPost("/requestUpdatePasswordAppUser",
                     obj,
                     defaultReplyHandler,
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
    MACRO_RAW_HEADER();
    yacappServerGet(methodNames.getWorktimeState,
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);

}

void YACServerNetwork::appUserInsertWorktime(const QString &appId,
                                             const QString &loginEMail,
                                             const QString &loginToken,
                                             const int worktimeType,
                                             const int userMood,
                                             const int dayRating,
                                             const QDateTime ts,
                                             JSONCallbackFunction jsonSuccessCallback,
                                             CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    obj["worktimeType"] = worktimeType;
    obj["userMood"] = userMood;
    obj["dayRating"] = dayRating;
    obj["ts"] = ts.toTimeSpec(Qt::LocalTime).toString(Qt::ISODate);

    yacappServerPost(methodNames.insertWorktime,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     0,
                     jsonSuccessCallback,
                     errorCallback);


}

void YACServerNetwork::appUserFetchWorktimes(const QString &appId,
                                             const QString &loginEMail,
                                             const QString &loginToken,
                                             const QDateTime &since,
                                             const QDateTime &until,
                                             JSONCallbackFunction successCallback,
                                             CallbackFunction errorCallback)
{
    QUrlQuery query;
    query.addQueryItem("sinceISO", since.toString(Qt::ISODate));
    query.addQueryItem("untilISO", until.toString(Qt::ISODate));
    MACRO_RAW_HEADER();
    yacappServerGet(methodNames.fetchWorktimes,
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    successCallback,
                    errorCallback);
}

void YACServerNetwork::appUserInsertWorktimeBeginEnd(const QString &appId,
                                                     const QString &loginEMail,
                                                     const QString &loginToken,
                                                     const int worktimeType,
                                                     const QDateTime begin,
                                                     const QDateTime end,
                                                     CallbackFunction successCallback,
                                                     CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    obj["worktimeType"] = worktimeType;
    obj["beginISO"] = begin.toTimeSpec(Qt::LocalTime).toString(Qt::ISODate);
    obj["endISO"] = end.toTimeSpec(Qt::LocalTime).toString(Qt::ISODate);;

    yacappServerPost(methodNames.insertWorktimeBeginEnd,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     successCallback,
                     0,
                     errorCallback);
}

void YACServerNetwork::appUserDeleteWorktime(const QString &appId,
                                             const QString &loginEMail,
                                             const QString &loginToken,
                                             const QString &id,
                                             CallbackFunction successCallback,
                                             CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    obj["id"] = id;

    yacappServerPost(methodNames.deleteWorktime,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     successCallback,
                     0,
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
    MACRO_RAW_HEADER();
    yacappServerGet("/appUserSearchProfiles",
                    query,
                    defaultReplyHandler,
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
    MACRO_RAW_HEADER();

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
    MACRO_RAW_HEADER();

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

void YACServerNetwork::appUserUpdateProfile(const QString &appId,
                                            const QString &loginEMail,
                                            const QString &loginToken,
                                            const QString &fstname,
                                            const QString &surname,
                                            const QString &visible_name,
                                            const QString &profileFilename,
                                            const bool searching_exactly_allowed,
                                            const bool searching_fuzzy_allowed,
                                            const QString &password,
                                            JSONCallbackFunction successCallback,
                                            CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    obj["fstname"] = fstname;
    obj["surname"] = surname;
    obj["visible_name"] = visible_name;
    obj["searching_exactly_allowed"] = searching_exactly_allowed;
    obj["searching_fuzzy_allowed"] = searching_fuzzy_allowed;
    MACRO_JSON_SET(obj, password);
    if (profileFilename.size())
    {
        QFile file(profileFilename);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray image(file.readAll());
            obj["with_image"] = true;
            obj["image_data_base64"] = QString(image.toBase64());
        }
        else
        {
            obj["with_image"] = false;
        }
    }

    yacappServerPost("/updateAppUserProfile",
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     0,
                     successCallback,
                     errorCallback);
}

void YACServerNetwork::appUserFetchMessageUpdates(const QString &appId,
                                                  const QString &loginEMail,
                                                  const QString &loginToken,
                                                  const QDateTime &updatesSince,
                                                  JSONCallbackFunction jsonSuccessCallback,
                                                  CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QUrlQuery query;
    QString help(updatesSince.toString(Qt::ISODate));
    query.addQueryItem("updatesSinceISO", updatesSince.toString(Qt::ISODate));

    yacappServerGet("/fetchMessageUpdates",
                    query,
                    defaultReplyHandler,
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

void YACServerNetwork::appUserFetchMyProfile(const QString &appId,
                                             const QString &loginEMail,
                                             const QString &loginToken,
                                             JSONCallbackFunction jsonSuccessCallback,
                                             CallbackFunction errorCallback)
{
    QUrlQuery query;

    yacappServerAppUserGetJSONReply("/fetchMyProfile",
                                    query,
                                    appId,
                                    loginEMail,
                                    loginToken,
                                    jsonSuccessCallback,
                                    errorCallback);
}

void YACServerNetwork::appUserFetchImage(const QString &appId,
                                         const QString &loginEMail,
                                         const QString &loginToken,
                                         const QString &imageType,
                                         const QString &imageId,
                                         JSONCallbackFunction jsonSuccessCallback,
                                         CallbackFunction errorCallback)
{
    QUrlQuery query;
    query.addQueryItem("imageType", imageType);
    query.addQueryItem("imageId", imageId);
    MACRO_RAW_HEADER();
    yacappServerGet("/fetchImage",
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);
}

void YACServerNetwork::appUserInsertAppointment(const QString &appId,
                                                const QString &loginEMail,
                                                const QString &loginToken,
                                                const QString &appointment_group_id,
                                                const QString &appointment_template_id,
                                                const QString &caption,
                                                const QDateTime &start_datetime,
                                                const QDateTime &end_datetime,
                                                const bool visible_for_everybody,
                                                JSONCallbackFunction jsonSuccessCallback,
                                                CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    obj["appointment_group_id"] = appointment_group_id;
    obj["caption"] = caption;
    obj["start_datetime"] = start_datetime.toString(Qt::ISODate);
    obj["end_datetime"] = end_datetime.toString(Qt::ISODate);
    obj["appointment_template_id"] = appointment_template_id;
    obj["visible_for_everybody"] = visible_for_everybody;

    QDateTime bookable_since_datetime;
    obj["bookable_since_datetime"] = bookable_since_datetime.toString(Qt::ISODate);
    QDateTime bookable_until_datetime;
    obj["bookable_until_datetime"] = bookable_until_datetime.toString(Qt::ISODate);

    QString decription;
    obj["decription"] = decription;
    int max_bookable_count(0);
    obj["max_bookable_count"] = max_bookable_count;
    int booking_credits(0);
    obj["booking_credits"] = booking_credits;

    yacappServerPost(methodNames.insertAppointment,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     0,
                     jsonSuccessCallback,
                     errorCallback);
}

void YACServerNetwork::appUserFetchAppointments(const QString &appId,
                                                const QString &loginEMail,
                                                const QString &loginToken,
                                                JSONCallbackFunction jsonSuccessCallback,
                                                CallbackFunction errorCallback)
{
    QUrlQuery query;
    MACRO_RAW_HEADER();
    yacappServerGet(methodNames.fetchAppointments,
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);
}

void YACServerNetwork::appUserDeleteAppointment(const QString &appId,
                                                const QString &loginEMail,
                                                const QString &loginToken,
                                                const QString &id,
                                                CallbackFunction successCallback,
                                                CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();
    QJsonObject obj;
    obj[tableFields.id] = id;
    yacappServerPost(methodNames.deleteAppointment,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     successCallback,
                     0,
                     errorCallback);
}

void YACServerNetwork::appUserFetchRightGroups(const QString &appId,
                                               const QString &loginEMail,
                                               const QString &loginToken,
                                               JSONCallbackFunction jsonSuccessCallback,
                                               CallbackFunction errorCallback)
{
    QUrlQuery query;
    MACRO_RAW_HEADER();
    yacappServerGet(methodNames.fetchRightGroups,
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);
}

void YACServerNetwork::appUserInsertOrUpdateRightGroup(const QString &appId,
                                                       const QString &loginEMail,
                                                       const QString &loginToken,
                                                       const QString &id,
                                                       const QString &name,
                                                       const bool automatic,
                                                       const QString &access_code,
                                                       const bool request_allowed,
                                                       const bool visible_for_non_members,
                                                       JSONCallbackFunction jsonSuccessCallback,
                                                       CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    MACRO_JSON_SET(obj, name);
    MACRO_JSON_SET(obj, automatic);
    MACRO_JSON_SET(obj, access_code);
    MACRO_JSON_SET(obj, request_allowed);
    MACRO_JSON_SET(obj, visible_for_non_members);
    if (id.size())
    {
        MACRO_JSON_SET(obj, id);
    }
    yacappServerPost(id.size() ? methodNames.updateRightGroup : methodNames.insertRightGroup,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     0,
                     jsonSuccessCallback,
                     errorCallback);

}

void YACServerNetwork::appUserDeleteRightGroup(const QString &appId, const QString &loginEMail, const QString &loginToken, const QString &id, CallbackFunction successCallback, CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    MACRO_JSON_SET(obj, id);
    yacappServerPost(methodNames.deleteRightGroup,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     successCallback,
                     0,
                     errorCallback);
}

void YACServerNetwork::appUserFetchRightGroup(const QString &appId,
                                              const QString &loginEMail,
                                              const QString &loginToken,
                                              const QString &id,
                                              JSONCallbackFunction jsonSuccessCallback,
                                              CallbackFunction errorCallback)
{
    QUrlQuery query;
    query.addQueryItem("id", id);
    MACRO_RAW_HEADER();
    yacappServerGet(methodNames.fetchRightGroup,
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);

}

void YACServerNetwork::appUserFetchSpaces(const QString &appId, const QString &loginEMail, const QString &loginToken, JSONCallbackFunction jsonSuccessCallback, CallbackFunction errorCallback)
{
    QUrlQuery query;
    MACRO_RAW_HEADER();
    yacappServerGet(methodNames.fetchSpaces,
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);
}

void YACServerNetwork::appUserInsertOrUpdateSpace(const QString &appId,
                                                  const QString &loginEMail,
                                                  const QString &loginToken,
                                                  const QString &id,
                                                  const QString &name,
                                                  const QString &access_code,
                                                  const bool automatic,
                                                  const bool request_allowed,
                                                  JSONCallbackFunction jsonSuccessCallback,
                                                  CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    MACRO_JSON_SET(obj, name);
    MACRO_JSON_SET(obj, access_code);
    MACRO_JSON_SET(obj, automatic);
    MACRO_JSON_SET(obj, request_allowed);
    if (id.size())
    {
        MACRO_JSON_SET(obj, id);
    }
    yacappServerPost(id.size() ? methodNames.updateSpace : methodNames.insertSpace,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     0,
                     jsonSuccessCallback,
                     errorCallback);
}

void YACServerNetwork::appUserDeleteSpace(const QString &appId, const QString &loginEMail, const QString &loginToken, const QString &id, CallbackFunction successCallback, CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    MACRO_JSON_SET(obj, id);
    yacappServerPost(methodNames.deleteSpace,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     successCallback,
                     0,
                     errorCallback);
}

void YACServerNetwork::appUserFetchSpace(const QString &appId, const QString &loginEMail, const QString &loginToken, const QString &id, JSONCallbackFunction jsonSuccessCallback, CallbackFunction errorCallback)
{
    QUrlQuery query;
    query.addQueryItem("id", id);
    MACRO_RAW_HEADER();
    yacappServerGet(methodNames.fetchSpace,
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);
}

void YACServerNetwork::appUserRequestSpaceAccess(const QString &appId,
                                                 const QString &loginEMail,
                                                 const QString &loginToken,
                                                 const QString &space_id,
                                                 CallbackFunction successCallback,
                                                 CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    QJsonObject obj;
    MACRO_JSON_SET(obj, space_id);
    yacappServerPost(methodNames.requestSpaceAccess,
                     obj,
                     defaultReplyHandler,
                     rawHeader,
                     successCallback,
                     0,
                     errorCallback);

}
