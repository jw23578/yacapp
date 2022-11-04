#include "networkinterface.h"

NetworkInterface::NetworkInterface(QNetworkAccessManager &manager
                                   , const Constants &constants
                                   , QObject *parent)
    : QObject{parent}
    , manager(manager)
    , constants(constants)
{
    connect(&manager, &QNetworkAccessManager::finished,
            this, &NetworkInterface::replyFinished);

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
    rr.handlerFunction(reply, rr);
}
