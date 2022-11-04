#ifndef YACSERVERBASENETWORK_H
#define YACSERVERBASENETWORK_H

#include "networkinterface.h"

class YACServerBaseNetwork : public NetworkInterface
{
protected:
    const QString yacappServerUrl = {"http://www.jw78.de:23579"}; // {"http://127.0.0.1:23578"}; //

    void yacappServerPost(const QString &method,
                          const QJsonObject &object,
                          HandlerFunction handlerFunction,
                          const QMap<QByteArray, QByteArray> &rawHeader,
                          CallbackFunction registerCallback,
                          CallbackFunction errorCallback);

    void yacappServerPost(const QString &method,
                          const QJsonObject &object,
                          HandlerFunction handlerFunction,
                          CallbackFunction registerCallback,
                          CallbackFunction errorCallback);

    void yacappServerGet(const QString &method,
                         const QUrlQuery &query,
                         HandlerFunction handlerFunction,
                         CallbackFunction registerCallback,
                         CallbackFunction errorCallback);

public:
    explicit YACServerBaseNetwork(QNetworkAccessManager &manager
                                  , const Constants &constants);
};

#endif // YACSERVERBASENETWORK_H
