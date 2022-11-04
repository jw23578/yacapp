#include "customservernetwork.h"
#include <QFile>
#include <QFileInfo>

CustomServerNetwork::CustomServerNetwork(QNetworkAccessManager &manager
                                         , const Constants &constants)
    : NetworkInterface{manager
                       , constants}
{

}

void CustomServerNetwork::downloadApp(QString projectFilename,
                             QString projectPackage,
                             std::function<void(const QString &)> appDownloadedCallback,
                             std::function<void(const QString &errorMessage)> errorCallback)
{
    QNetworkRequest request;
    request.setUrl(QUrl(projectFilename));
    QNetworkReply *reply(manager.get(request));
    SRunningRequest &rr(runningRequests[reply]);
    rr.handlerFunction = std::bind(&CustomServerNetwork::projectFilenameFinished, this, std::placeholders::_1, std::placeholders::_2);
    rr.projectFilename = projectFilename;
    rr.projectPackage = projectPackage;
    rr.errorCallback = errorCallback;
    rr.successCallback = appDownloadedCallback;
}

void CustomServerNetwork::projectFilenameFinished(QNetworkReply *finishedReply, SRunningRequest &rr)
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
    rr.handlerFunction = std::bind(&CustomServerNetwork::projectPackageFinished, this, std::placeholders::_1, std::placeholders::_2);
    runningRequests[reply] = rr;
}

void CustomServerNetwork::projectPackageFinished(QNetworkReply *finishedReply, SRunningRequest &rr)
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
