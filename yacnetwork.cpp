#include "yacnetwork.h"
#include <QNetworkReply>

void YACNetwork::projectFilenameFinished(QNetworkReply *reply)
{

}

YACNetwork::YACNetwork():QObject(0)
{
    connect(&manager, &QNetworkAccessManager::finished,
            this, &YACNetwork::replyFinished);
}

void YACNetwork::fetchAllFiles(const QString &baseUrl,
                               const QString &projectFilename,
                               std::function<void ()> allLoadedCallback)
{
    QNetworkRequest request;
    request.setUrl(QUrl(baseUrl + projectFilename));
    QNetworkReply *reply(manager.get(request));
    reply2callback[reply] = std::bind(&YACNetwork::projectFilenameFinished, this, std::placeholders::_1);
}

void YACNetwork::replyFinished(QNetworkReply *reply)
{
    QMap<QNetworkReply*, replyCallbackFunction>::Iterator it(reply2callback.find(reply));
    if (it == reply2callback.end())
    {
        // error
        return;
    }
    replyCallbackFunction callback(it.value());
    reply2callback.erase(it);
    callback(reply);
}
