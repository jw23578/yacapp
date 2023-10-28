#ifndef YACSERVERBASENETWORK_H
#define YACSERVERBASENETWORK_H

#include "networkinterface.h"

#define MACRO_RAW_HEADER() \
QMap<QByteArray, QByteArray> rawHeader; \
    rawHeader["YACAPP-AppId"] = appId.toLatin1(); \
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1(); \
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();


#define MACRO_JSON_SET_DATETIME(obj, nameValue) \
obj[#nameValue] = nameValue.toString(Qt::ISODate);

#define MACRO_JSON_SET(obj, nameValue) \
obj[#nameValue] = nameValue;


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
                          CallbackFunction errorCallback,
                          const QString &trackerUuid = "");

    void yacappServerPost(const QString &method,
                          const QJsonObject &object,
                          HandlerFunction handlerFunction,
                          const QMap<QByteArray, QByteArray> &rawHeader,
                          CallbackFunction successCallback,
                          CallbackFunction errorCallback,
                          const QString &trackerUuid = "");

    void yacappServerPost(const QString &method,
                          const QJsonObject &object,
                          HandlerFunction handlerFunction,
                          CallbackFunction successCallback,
                          CallbackFunction errorCallback,
                          const QString &trackerUuid = "");

    void yacappServerGet(QString method,
                         const QUrlQuery &query,
                         HandlerFunction handlerFunction,
                         const QMap<QByteArray, QByteArray> &rawHeader,
                         CallbackFunction successCallback,
                         JSONCallbackFunction jsonSuccessCallback,
                         CallbackFunction errorCallback,
                         const QString &trackerUuid = "");

    void yacappServerGet(const QString &method,
                         const QUrlQuery &query,
                         HandlerFunction handlerFunction,
                         CallbackFunction successCallback,
                         CallbackFunction errorCallback,
                         const QString &trackerUuid = "");

    void yacappServerDelete(QString method,
                            const QUrlQuery &query,
                            HandlerFunction handlerFunction,
                            const QMap<QByteArray, QByteArray> &rawHeader,
                            JSONCallbackFunction jsonSuccessCallback,
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

    QString getYacappServerUrl() const;

    void yacappServerDelete(const QString &method,
                            const QUrlQuery &query,
                            const QString &appId,
                            const QString &loginEMail,
                            const QString &loginToken,
                            JSONCallbackFunction jsonSuccessCallback,
                            CallbackFunction errorCallback);

};

#endif // YACSERVERBASENETWORK_H
