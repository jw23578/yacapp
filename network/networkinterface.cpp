#include "networkinterface.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "logger.h"

NetworkInterface::NetworkInterface(QNetworkAccessManager &manager
                                   , Constants &constants
                                   , QObject *parent)
    : QObject{parent}
    , manager(manager)
    , constants(constants)
{
    finishedTracker.setPercentState(100);
    connect(&manager, &QNetworkAccessManager::finished,
            this, &NetworkInterface::replyFinished);

}

void NetworkInterface::addRunningRequest(QNetworkReply *reply, const SRunningRequest &rr)
{
    if (rr.trackerUuid.size())
    {
        TransmissionTracker *tt(tracker(rr.trackerUuid));
        tt->setActive(true);
        connect(reply, &QNetworkReply::downloadProgress, tt, &TransmissionTracker::downloadProgress);
        connect(reply, &QNetworkReply::uploadProgress, tt, &TransmissionTracker::uploadProgress);
    }
    runningRequests[reply] = rr;
}


TransmissionTracker *NetworkInterface::tracker(const QString &uuid)
{
    if (uuid.size() == 0)
    {
        return &finishedTracker;
    }
    auto it(uuid2TransmissionTracker.find(uuid));
    if (it == uuid2TransmissionTracker.end())
    {
        TransmissionTracker *tt (new TransmissionTracker);
        uuid2TransmissionTracker[uuid] = tt;
        return tt;
    }
    return it->second;
}

void defaultReplyHandler(QNetworkReply *finishedReply, QByteArray &allData, NetworkInterface::SRunningRequest &rr)
{
    QJsonDocument replyDoc(QJsonDocument::fromJson(allData));
    QJsonObject object(replyDoc.object());
    if (object["success"].toBool())
    {
        if (rr.successCallback)
        {
            QString message(object["message"].toString());
            rr.successCallback(message);
        }
        if (rr.jsonSuccessCallback)
        {
            rr.jsonSuccessCallback(replyDoc);
        }
    }
    else
    {
        int missingRight(object["missingRight"].toInt());
        if (missingRight)
        {
            emit rr.networkInterface->missingRight(missingRight);
        }
        QString message(object["message"].toString());
        rr.errorCallback(message);
    }
}

void NetworkInterface::replyFinished(QNetworkReply *reply)
{
    QMap<QNetworkReply*, SRunningRequest>::Iterator it(runningRequests.find(reply));
    if (it == runningRequests.end())
    {
        // error
        return;
    }
    SRunningRequest rr(it.value());
    if (rr.trackerUuid.size())
    {
        TransmissionTracker *tt(tracker(rr.trackerUuid));
        tt->setActive(false);
    }
    runningRequests.erase(it);
    QByteArray allData(reply->readAll());

    ONLY_DESKTOP_LOG(allData.left(1024));

    if (reply->error() != QNetworkReply::NoError)
    {
        if (networkDefectCallback)
        {
            networkDefectCallback(reply->errorString());
        }
        DEFAULT_LOG(reply->errorString());
        DEFAULT_LOG(QString("Url: ") + reply->url().toString());
        QJsonDocument replyDoc(QJsonDocument::fromJson(allData));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        if (message.size())
        {
            rr.errorCallback(message);
            return;
        }
        rr.errorCallback(reply->errorString());
        return;
    }
    if (networkGoodCallback)
    {
        networkGoodCallback();
    }
    rr.networkInterface = this;
    rr.handlerFunction(reply, allData, rr);
}
