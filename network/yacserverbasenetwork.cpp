#include "yacserverbasenetwork.h"
#include <QUrlQuery>
#include <QJsonDocument>

YACServerBaseNetwork::YACServerBaseNetwork(QNetworkAccessManager &manager
                                           , const Constants &constants):
    NetworkInterface{manager, constants}
{

}

void YACServerBaseNetwork::yacappServerPost(const QString &method,
                                            const QJsonObject &object,
                                            HandlerFunction handlerFunction,
                                            const QMap<QByteArray, QByteArray> &rawHeader,
                                            CallbackFunction successCallback,
                                            JSONCallbackFunction jsonSuccessCallback,
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
    rr.successCallback = successCallback;
    rr.jsonSuccessCallback = jsonSuccessCallback;

}

void YACServerBaseNetwork::yacappServerPost(const QString &method,
                                            const QJsonObject &object,
                                            HandlerFunction handlerFunction,
                                            const QMap<QByteArray, QByteArray> &rawHeader,
                                            CallbackFunction registerCallback,
                                            CallbackFunction errorCallback)
{
    yacappServerPost(method,
                     object,
                     handlerFunction,
                     rawHeader,
                     registerCallback,
                     0,
                     errorCallback);
}
void YACServerBaseNetwork::yacappServerPost(const QString &method,
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

void YACServerBaseNetwork::yacappServerGet(const QString &method,
                                           const QUrlQuery &query,
                                           HandlerFunction handlerFunction,
                                           const QMap<QByteArray, QByteArray> &rawHeader,
                                           CallbackFunction successCallback,
                                           JSONCallbackFunction jsonSuccessCallback,
                                           CallbackFunction errorCallback)
{
    QUrl url(yacappServerUrl + method);
    url.setQuery(query.query());
    QNetworkRequest request;
    request.setUrl(url);
    QMap<QByteArray, QByteArray>::ConstIterator it(rawHeader.begin());
    while (it != rawHeader.end())
    {
        request.setRawHeader(it.key(), it.value());
        ++it;
    }

    QNetworkReply *reply(manager.get(request));

    SRunningRequest &rr(runningRequests[reply]);
    rr.handlerFunction = handlerFunction;
    rr.errorCallback = errorCallback;
    rr.successCallback = successCallback;
    rr.jsonSuccessCallback = jsonSuccessCallback;
}

void YACServerBaseNetwork::yacappServerGet(const QString &method,
                                           const QUrlQuery &query,
                                           HandlerFunction handlerFunction,
                                           CallbackFunction successCallback,
                                           CallbackFunction errorCallback)
{
    yacappServerGet(method,
                    query,
                    handlerFunction,
                    QMap<QByteArray, QByteArray>(),
                    successCallback,
                    0,
                    errorCallback);
}

