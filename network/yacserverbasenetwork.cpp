#include "yacserverbasenetwork.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include "logger.h"

QString YACServerBaseNetwork::yacappServerUrl  = {"https://www.jw78.de:23579"}; // {"http://127.0.0.1:23578"}; //

YACServerBaseNetwork::YACServerBaseNetwork(QNetworkAccessManager &manager
                                           , Constants &constants):
    NetworkInterface{manager, constants}
{

}

void YACServerBaseNetwork::setThirdMandant(const QString &third,
                                           const QString &mandant)
{
    this->third = third;
    this->mandant = mandant;
}

QString YACServerBaseNetwork::getYacappServerUrl() const
{
    return yacappServerUrl;
}

void YACServerBaseNetwork::yacappServerPost(QString method,
                                            const QJsonObject &object,
                                            HandlerFunction handlerFunction,
                                            const QMap<QByteArray, QByteArray> &rawHeader,
                                            CallbackFunction successCallback,
                                            JSONCallbackFunction jsonSuccessCallback,
                                            CallbackFunction errorCallback,
                                            const QString &trackerUuid)
{
    if (method[0] != '/')
    {
        method = "/" + method;
    }
    QNetworkRequest request;
    request.setUrl(QUrl(yacappServerUrl + method));
    QJsonDocument doc(object);
    QByteArray postData(doc.toJson());
    ONLY_DESKTOP_LOG(request.url().toString());
    ONLY_DESKTOP_LOG(postData.left(1000));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    QMap<QByteArray, QByteArray>::ConstIterator it(rawHeader.begin());
    while (it != rawHeader.end())
    {
        request.setRawHeader(it.key(), it.value());
        ++it;
    }
    QNetworkReply *reply(manager.post(request, postData));

    SRunningRequest rr;
    rr.trackerUuid = trackerUuid;
    rr.handlerFunction = handlerFunction;
    rr.errorCallback = errorCallback;
    rr.successCallback = successCallback;
    rr.jsonSuccessCallback = jsonSuccessCallback;
    addRunningRequest(reply, rr);
}

void YACServerBaseNetwork::yacappServerPost(const QString &method,
                                            const QJsonObject &object,
                                            HandlerFunction handlerFunction,
                                            const QMap<QByteArray, QByteArray> &rawHeader,
                                            CallbackFunction successCallback,
                                            CallbackFunction errorCallback,
                                            const QString &trackerUuid)
{
    yacappServerPost(method,
                     object,
                     handlerFunction,
                     rawHeader,
                     successCallback,
                     0,
                     errorCallback,
                     trackerUuid);
}
void YACServerBaseNetwork::yacappServerPost(const QString &method,
                                            const QJsonObject &object,
                                            HandlerFunction handlerFunction,
                                            CallbackFunction successCallback,
                                            CallbackFunction errorCallback,
                                            const QString &trackerUuid)
{
    yacappServerPost(method,
                     object,
                     handlerFunction,
                     QMap<QByteArray, QByteArray>(),
                     successCallback,
                     errorCallback,
                     trackerUuid);
}

void YACServerBaseNetwork::yacappServerGet(QString method,
                                           const QUrlQuery &query,
                                           HandlerFunction handlerFunction,
                                           const QMap<QByteArray, QByteArray> &rawHeader,
                                           CallbackFunction successCallback,
                                           JSONCallbackFunction jsonSuccessCallback,
                                           CallbackFunction errorCallback,
                                           const QString &trackerUuid)
{
    if (method[0] != '/')
    {
        method = "/" + method;
    }
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

    SRunningRequest rr;
    rr.trackerUuid = trackerUuid;
    rr.handlerFunction = handlerFunction;
    rr.errorCallback = errorCallback;
    rr.successCallback = successCallback;
    rr.jsonSuccessCallback = jsonSuccessCallback;
    addRunningRequest(reply, rr);
}

void YACServerBaseNetwork::yacappServerGet(const QString &method,
                                           const QUrlQuery &query,
                                           HandlerFunction handlerFunction,
                                           CallbackFunction successCallback,
                                           CallbackFunction errorCallback,
                                           const QString &trackerUuid)
{
    yacappServerGet(method,
                    query,
                    handlerFunction,
                    QMap<QByteArray, QByteArray>(),
                    successCallback,
                    0,
                    errorCallback,
                    trackerUuid);
}

void YACServerBaseNetwork::yacappServerDelete(QString method,
                                              const QUrlQuery &query,
                                              HandlerFunction handlerFunction,
                                              const QMap<QByteArray, QByteArray> &rawHeader,
                                              JSONCallbackFunction jsonSuccessCallback,
                                              CallbackFunction errorCallback)
{
    if (method[0] != '/')
    {
        method = "/" + method;
    }
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

    QNetworkReply *reply(manager.deleteResource(request));

    SRunningRequest rr;
    rr.handlerFunction = handlerFunction;
    rr.errorCallback = errorCallback;
    rr.jsonSuccessCallback = jsonSuccessCallback;
    addRunningRequest(reply, rr);
}

void YACServerBaseNetwork::yacappServerDelete(const QString &method,
                                              const QUrlQuery &query,
                                              const QString &appId,
                                              const QString &loginEMail,
                                              const QString &loginToken,
                                              JSONCallbackFunction jsonSuccessCallback,
                                              CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();
    yacappServerDelete(method,
                       query,
                       defaultReplyHandler,
                       rawHeader,
                       jsonSuccessCallback,
                       errorCallback);
}

void YACServerBaseNetwork::yacappServerAppUserGetJSONReply(const QString &method,
                                                           const QUrlQuery &query,
                                                           const QString &appId,
                                                           const QString &loginEMail,
                                                           const QString &loginToken,
                                                           JSONCallbackFunction jsonSuccessCallback,
                                                           CallbackFunction errorCallback)
{
    MACRO_RAW_HEADER();

    yacappServerGet(method,
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);

}

