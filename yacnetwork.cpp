#include "yacnetwork.h"
#include <QNetworkReply>
#include <QFileInfo>
#include <QDir>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

void YACNetwork::projectFilenameFinished(QNetworkReply *finishedReply, SRunningRequest &rr)
{
    if (finishedReply->error() != QNetworkReply::NoError)
    {
        rr.errorCallback(tr("Error on downloading Projectfile: ") + finishedReply->errorString());
        return;
    }
    // Success? Then save Project File

    QFile file(constants.getYacAppConfigPath() + QFileInfo(rr.projectFilename).fileName());
    file.open(QIODevice::WriteOnly);
    file.write(finishedReply->readAll());
    file.close();

    QNetworkRequest request;
    request.setUrl(QUrl(rr.projectPackage));
    QNetworkReply *reply(manager.get(request));
    rr.handlerFunction = std::bind(&YACNetwork::projectPackageFinished, this, std::placeholders::_1, std::placeholders::_2);
    runningRequests[reply] = rr;
}

void YACNetwork::projectPackageFinished(QNetworkReply *finishedReply, SRunningRequest &rr)
{
    if (finishedReply->error() != QNetworkReply::NoError)
    {
        rr.errorCallback(tr("Error on downloading Projectfile: ") + finishedReply->errorString());
        return;
    }
    QByteArray uncompressedData(qUncompress(finishedReply->readAll()));

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
    rr.successCallback("");
}

YACNetwork::YACNetwork(const Constants &constants):QObject(0),
    constants(constants)
{
    connect(&manager, &QNetworkAccessManager::finished,
            this, &YACNetwork::replyFinished);
}

void YACNetwork::downloadApp(QString projectFilename,
                             QString projectPackage,
                             std::function<void(const QString &)> appDownloadedCallback,
                             std::function<void(const QString &errorMessage)> errorCallback)
{
    QNetworkRequest request;
    request.setUrl(QUrl(projectFilename));
    QNetworkReply *reply(manager.get(request));
    SRunningRequest &rr(runningRequests[reply]);
    rr.handlerFunction = std::bind(&YACNetwork::projectFilenameFinished, this, std::placeholders::_1, std::placeholders::_2);
    rr.projectFilename = projectFilename;
    rr.projectPackage = projectPackage;
    rr.errorCallback = errorCallback;
    rr.successCallback = appDownloadedCallback;
}

void YACNetwork::yacappServerPost(const QString &method,
                                  const QJsonObject &object,
                                  HandlerFunction handlerFunction,
                                  const QMap<QByteArray, QByteArray> &rawHeader,
                                  CallbackFunction registerCallback,
                                  CallbackFunction errorCallback)
{
    QNetworkRequest request;
    request.setUrl(QUrl(yacappServerUrl + method));
    QJsonDocument doc(object);
    QByteArray postData(doc.toJson());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    QMap<QByteArray, QByteArray>::ConstIterator it(rawHeader.begin());
    while (it != rawHeader.end())
    {
        request.setRawHeader(it.key(), it.value());
        ++it;
    }
    QNetworkReply *reply(manager.post(request, postData));

    SRunningRequest &rr(runningRequests[reply]);
    rr.handlerFunction = handlerFunction;
    rr.errorCallback = errorCallback;
    rr.successCallback = registerCallback;
}
void YACNetwork::yacappServerPost(const QString &method,
                                  const QJsonObject &object,
                                  HandlerFunction handlerFunction,
                                  CallbackFunction registerCallback,
                                  CallbackFunction errorCallback)
{
    yacappServerPost(method,
                     object,
                     handlerFunction,
                     QMap<QByteArray, QByteArray>(),
                     registerCallback,
                     errorCallback);
}

void YACNetwork::yacappServerGet(const QString &method,
                                 const QUrlQuery &query,
                                 HandlerFunction handlerFunction,
                                 CallbackFunction registerCallback,
                                 CallbackFunction errorCallback)
{
    QUrl url(yacappServerUrl + method);
    url.setQuery(query.query());
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply(manager.get(request));

    SRunningRequest &rr(runningRequests[reply]);
    rr.handlerFunction = handlerFunction;
    rr.errorCallback = errorCallback;
    rr.successCallback = registerCallback;
}

void YACNetwork::yacappServerRegisterUser(QString loginEMail,
                                          QString password,
                                          CallbackFunction successCallback,
                                          CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply,
                      SRunningRequest &rr)
    {
        qDebug() << finishedReply->readAll();
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

void YACNetwork::yacappServerVerifyUser(QString loginEMail, QString verifyToken, CallbackFunction successCallback, CallbackFunction errorCallback)
{
    auto replyHandler([](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        QByteArray all(finishedReply->readAll());
        qDebug() << all;
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

void YACNetwork::yacappServerLoginUser(QString loginEMail, QString password, CallbackFunction successCallback, CallbackFunction errorCallback)
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

void YACNetwork::yacappServerUserLoggedIn(QString loginEMail, QString verifyToken, CallbackFunction successCallback, CallbackFunction errorCallback)
{
    auto replyHander = [](QNetworkReply *finishedReply, SRunningRequest &rr)
    {
        qDebug() << finishedReply->readAll();
    };

    QUrlQuery query;
    query.addQueryItem("loginEMail", loginEMail);
    query.addQueryItem("verifyToken", verifyToken);
    yacappServerGet("/userLoggedIn", query,
                    replyHander,
                    successCallback,
                    errorCallback);
}

void YACNetwork::yacappServerUploadApp(const QString &loginEMail,
                                       const QString &loginToken,
                                       const QString &app_id,
                                       const QString &app_name,
                                       const QString &app_logo_url,
                                       const QString &app_color_name,
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
    obj["app_logo_url"] = app_logo_url;
    obj["app_color_name"] = app_color_name;
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

void YACNetwork::yacappServerGetAllAPPs(CallbackFunction successCallback,
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

void YACNetwork::replyFinished(QNetworkReply *reply)
{
    QMap<QNetworkReply*, SRunningRequest>::Iterator it(runningRequests.find(reply));
    if (it == runningRequests.end())
    {
        // error
        return;
    }
    SRunningRequest rr(it.value());
    runningRequests.erase(it);
    rr.handlerFunction(reply, rr);
}
