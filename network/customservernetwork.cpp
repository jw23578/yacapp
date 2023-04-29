#include "customservernetwork.h"
#include <QFile>
#include <QFileInfo>

CustomServerNetwork::CustomServerNetwork(QNetworkAccessManager &manager
                                         , Constants &constants)
    : NetworkInterface{manager
                       , constants}
{

}

void CustomServerNetwork::projectFilenameFinished(QNetworkReply *finishedReply, QByteArray &allData, SRunningRequest &rr)
{
    if (finishedReply->error() != QNetworkReply::NoError)
    {
        rr.errorCallback(tr("Error on downloading Projectfile: ") + finishedReply->errorString());
        return;
    }
    // Success? Then save Project File

    QFile file(constants.getYacAppConfigPath(rr.appId) + QFileInfo(rr.projectFilename).fileName());
    file.open(QIODevice::WriteOnly);
    file.write(allData);
    file.close();

    QNetworkRequest request;
    request.setUrl(QUrl(rr.projectPackage));
    QNetworkReply *reply(manager.get(request));
    rr.handlerFunction = std::bind(&CustomServerNetwork::projectPackageFinished, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    runningRequests[reply] = rr;
}

void CustomServerNetwork::projectPackageFinished(QNetworkReply *finishedReply, QByteArray &allData, SRunningRequest &rr)
{
    if (finishedReply->error() != QNetworkReply::NoError)
    {
        rr.errorCallback(tr("Error on downloading Projectfile: ") + finishedReply->errorString());
        return;
    }
    QByteArray uncompressedData(qUncompress(allData));

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

        QFile file(constants.getYacAppConfigPath(rr.appId) + filename);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        pos = nextPos + 1;
    }
    rr.successCallback("");
}
