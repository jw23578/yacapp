#include "yacserverbasenetwork.h"
#include <QUrlQuery>
#include <QJsonDocument>

QString YACServerBaseNetwork::yacappServerUrl  = {"http://www.jw78.de:23579"}; // {"http://127.0.0.1:23578"}; //

YACServerBaseNetwork::YACServerBaseNetwork(QNetworkAccessManager &manager
                                           , Constants &constants):
    NetworkInterface{manager, constants}
{

}

void YACServerBaseNetwork::yacappServerPost(QString method,
                                            const QJsonObject &object,
                                            HandlerFunction handlerFunction,
                                            const QMap<QByteArray, QByteArray> &rawHeader,
                                            CallbackFunction successCallback,
                                            JSONCallbackFunction jsonSuccessCallback,
                                            CallbackFunction errorCallback)
{
    if (method[0] != '/')
    {
        method = "/" + method;
    }
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
                                            CallbackFunction successCallback,
                                            CallbackFunction errorCallback)
{
    yacappServerPost(method,
                     object,
                     handlerFunction,
                     rawHeader,
                     successCallback,
                     0,
                     errorCallback);
}
void YACServerBaseNetwork::yacappServerPost(const QString &method,
                                            const QJsonObject &object,
                                            HandlerFunction handlerFunction,
                                            CallbackFunction successCallback,
                                            CallbackFunction errorCallback)
{
    yacappServerPost(method,
                     object,
                     handlerFunction,
                     QMap<QByteArray, QByteArray>(),
                     successCallback,
                     errorCallback);
}

void YACServerBaseNetwork::yacappServerGet(QString method,
                                           const QUrlQuery &query,
                                           HandlerFunction handlerFunction,
                                           const QMap<QByteArray, QByteArray> &rawHeader,
                                           CallbackFunction successCallback,
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

void YACServerBaseNetwork::yacappServerAppUserGetJSONReply(const QString &method,
                                                           const QUrlQuery &query,
                                                           const QString &appId,
                                                           const QString &loginEMail,
                                                           const QString &loginToken,
                                                           JSONCallbackFunction jsonSuccessCallback,
                                                           CallbackFunction errorCallback)
{
    QMap<QByteArray, QByteArray> rawHeader;
    rawHeader["YACAPP-AppId"] = appId.toLatin1();
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();

    yacappServerGet(method,
                    query,
                    defaultReplyHandler,
                    rawHeader,
                    0,
                    jsonSuccessCallback,
                    errorCallback);

}

