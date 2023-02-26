#ifndef YACSERVERBASENETWORK_H
#define YACSERVERBASENETWORK_H

#include "networkinterface.h"

class YACServerBaseNetwork : public NetworkInterface
{
    friend int main(int argc, char *argv[]);
protected:
    static QString yacappServerUrl;

    void yacappServerPost(QString method,
                          const QJsonObject &object,
                          HandlerFunction handlerFunction,
                          const QMap<QByteArray, QByteArray> &rawHeader,
                          CallbackFunction successCallback,
                          JSONCallbackFunction jsonSuccessCallback,
                          CallbackFunction errorCallback);

    void yacappServerPost(const QString &method,
                          const QJsonObject &object,
                          HandlerFunction handlerFunction,
                          const QMap<QByteArray, QByteArray> &rawHeader,
                          CallbackFunction successCallback,
                          CallbackFunction errorCallback);

    void yacappServerPost(const QString &method,
                          const QJsonObject &object,
                          HandlerFunction handlerFunction,
                          CallbackFunction successCallback,
                          CallbackFunction errorCallback);

    void yacappServerGet(QString method,
                         const QUrlQuery &query,
                         HandlerFunction handlerFunction,
                         const QMap<QByteArray, QByteArray> &rawHeader,
                         CallbackFunction successCallback,
                         JSONCallbackFunction jsonSuccessCallback,
                         CallbackFunction errorCallback);

    void yacappServerGet(const QString &method,
                         const QUrlQuery &query,
                         HandlerFunction handlerFunction,
                         CallbackFunction successCallback,
                         CallbackFunction errorCallback);


    void yacappServerAppUserGetJSONReply(const QString &method,
                                         const QUrlQuery &query,
                                         const QString &appId,
                                         const QString &loginEMail,
                                         const QString &loginToken,
                                         JSONCallbackFunction jsonSuccessCallback,
                                         CallbackFunction errorCallback);
public:
    explicit YACServerBaseNetwork(QNetworkAccessManager &manager
                                  , Constants &constants);
};

#endif // YACSERVERBASENETWORK_H
