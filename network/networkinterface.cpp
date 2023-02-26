#include "networkinterface.h"
#include <QJsonDocument>
#include <QJsonObject>

NetworkInterface::NetworkInterface(QNetworkAccessManager &manager
                                   , Constants &constants
                                   , QObject *parent)
    : QObject{parent}
    , manager(manager)
    , constants(constants)
{
    connect(&manager, &QNetworkAccessManager::finished,
            this, &NetworkInterface::replyFinished);

}

void defaultReplyHandler(QNetworkReply *finishedReply, NetworkInterface::SRunningRequest &rr)
{
    QByteArray all(finishedReply->readAll());
    QJsonDocument replyDoc(QJsonDocument::fromJson(all));
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
    runningRequests.erase(it);
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << __FILE__ << " " << __LINE__ << ": " << reply->errorString();
        QByteArray all(reply->readAll());
        qDebug() << all;
        QJsonDocument replyDoc(QJsonDocument::fromJson(all));
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
    rr.handlerFunction(reply, rr);
}
