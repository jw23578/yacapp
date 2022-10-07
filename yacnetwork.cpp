#include "yacnetwork.h"
#include <QNetworkReply>

void YACNetwork::projectFilenameFinished(QNetworkReply *finishedReply, SRunningRequest &rr)
{
    if (finishedReply->error() != QNetworkReply::NoError)
    {
        rr.errorCallback(tr("Error on downloading Projectfile: ") + finishedReply->errorString());
        return;
    }
    // Success? Then save Project File

    QNetworkRequest request;
    request.setUrl(QUrl(rr.projectPackage));
    QNetworkReply *reply(manager.get(request));
    rr.handlerFunction = std::bind(&YACNetwork::projectPackageFinished, this, std::placeholders::_1, std::placeholders::_2);
    runningRequests[reply] = rr;
}

void YACNetwork::projectPackageFinished(QNetworkReply *finishedReply, SRunningRequest &rr)
{

}

YACNetwork::YACNetwork():QObject(0)
{
    connect(&manager, &QNetworkAccessManager::finished,
            this, &YACNetwork::replyFinished);
}

void YACNetwork::downloadApp(QString projectFilename,
                             QString projectPackage,
                             std::function<void()> appDownloadedCallback,
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
