#include "yacnetwork.h"
#include <QNetworkReply>
#include <QFileInfo>
#include <QDir>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

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

void YACNetwork::registerUserFinished(QNetworkReply *finishedReply,
                                      SRunningRequest &rr)
{
    qDebug() << finishedReply->readAll();
}

void YACNetwork::verifyUserFinished(QNetworkReply *finishedReply, SRunningRequest &rr)
{
    qDebug() << finishedReply->readAll();
}

void YACNetwork::loginUserFinished(QNetworkReply *finishedReply, SRunningRequest &rr)
{
    qDebug() << finishedReply->readAll();
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
                                  CallbackFunction registerCallback,
                                  CallbackFunction errorCallback)
{
    QNetworkRequest request;
    request.setUrl(QUrl(yacappServerUrl + method));
    QJsonDocument doc(object);
    QByteArray postData(doc.toJson());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    QNetworkReply *reply(manager.post(request, postData));

    SRunningRequest &rr(runningRequests[reply]);
    rr.handlerFunction = handlerFunction;
    rr.errorCallback = errorCallback;
    rr.successCallback = registerCallback;
}

void YACNetwork::yacappServerRegisterUser(QString loginEMail,
                                          QString password,
                                          CallbackFunction registerCallback,
                                          CallbackFunction errorCallback)
{
    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["password"] = password;
    yacappServerPost("/registerUser",
                     obj,
                     std::bind(&YACNetwork::registerUserFinished, this, std::placeholders::_1, std::placeholders::_2),
                     registerCallback,
                     errorCallback);
}

void YACNetwork::yacappServerVerifyUser(QString loginEMail, QString verifyToken, CallbackFunction registerCallback, CallbackFunction errorCallback)
{
    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["verifyToken"] = verifyToken;
    yacappServerPost("/verifyUser",
                     obj,
                     std::bind(&YACNetwork::verifyUserFinished, this, std::placeholders::_1, std::placeholders::_2),
                     registerCallback,
                     errorCallback);
}

void YACNetwork::yacappServerLoginUser(QString loginEMail, QString password, CallbackFunction registerCallback, CallbackFunction errorCallback)
{
    QJsonObject obj;
    obj["loginEMail"] = loginEMail;
    obj["password"] = password;
    yacappServerPost("/loginUser",
                     obj,
                     std::bind(&YACNetwork::loginUserFinished, this, std::placeholders::_1, std::placeholders::_2),
                     registerCallback,
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
