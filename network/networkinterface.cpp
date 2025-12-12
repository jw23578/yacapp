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

bool NetworkInterface::acceptedError(const QString &url)
{
    for (auto &it: urlPart2errorInfo)
    {
        if (url.contains(it.first))
        {
            AcceptErrorsInfo &aei(it.second);
            if (aei.currentCount >= aei.maxCount)
            {
                return false;
            }
            aei.currentCount += 1;
            return true;
        }
    }
    return false;
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

void NetworkInterface::acceptErrors(const QString &urlPart, size_t maxCount)
{
    urlPart2errorInfo[urlPart].maxCount = maxCount;
}

void defaultReplyHandler(QNetworkReply *finishedReply,
                         QByteArray &allData,
                         NetworkInterface::SRunningRequest &rr)
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

    DEFAULT_LOG(QString("Reply Finished Url: ") + reply->url().toString());
    DEFAULT_LOG(QString("Statuscode: ") + reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toString());
    ONLY_DESKTOP_LOG(allData.left(1024));
    auto header(reply->request().rawHeaderList());
    for (auto &h: header)
    {
        ONLY_DESKTOP_LOG(h + QString(": ") + reply->request().rawHeader(h));
    }
    auto headerPairs(reply->rawHeaderPairs());
    for (auto &hp: headerPairs)
    {
        ONLY_DESKTOP_LOG(hp.first + QString(": ") + hp.second);
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        DEFAULT_LOG(QString("Error on this url: ") + reply->url().toString());
        if (acceptedError(reply->url().toString()))
        {
            return;
        }
        DEFAULT_LOG(reply->errorString());
        QJsonDocument replyDoc(QJsonDocument::fromJson(allData));
        QJsonObject object(replyDoc.object());
        QString message(object["message"].toString());
        if (networkDefectCallback)
        {
            networkDefectCallback(reply->errorString() + " " + message);
        }
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
