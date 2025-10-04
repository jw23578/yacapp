#ifndef YACSERVERBASENETWORK_H
#define YACSERVERBASENETWORK_H

#include "networkinterface.h"
#include "extuuid.h"
#include "QJsonObject"

#define MACRO_RAW_HEADER() \
QMap<QByteArray, QByteArray> rawHeader; \
    rawHeader["YACAPP-AppId"] = appId.toLatin1(); \
    rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1(); \
    rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1(); \
    rawHeader["YACAPP-Third"] = third.toLatin1(); \
    rawHeader["YACAPP-Mandant"] = mandant.toLatin1();

#define RD_MACRO_RAW_HEADER() \
    rd.rawHeader["YACAPP-AppId"] = appId.toLatin1(); \
    rd.rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1(); \
    rd.rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1(); \
    rd.rawHeader["YACAPP-Third"] = third.toLatin1(); \
    rd.rawHeader["YACAPP-Mandant"] = mandant.toLatin1();


#define MACRO_JSON_SET_DATETIME(obj, nameValue) \
obj[#nameValue] = nameValue.toString(Qt::ISODate);

#define MACRO_JSON_SET_COLOR(obj, nameValue) \
obj[#nameValue] = nameValue.name()

#define MACRO_JSON_SET(obj, nameValue) \
                  obj[#nameValue] = nameValue;


class YACServerBaseNetwork : public NetworkInterface
{
    friend int main(int argc, char *argv[]);
protected:
    QString third;
    QString mandant;
    static QString yacappServerUrl;

public:
    class RequestData
    {
    private:
        RequestData(const RequestData &other) = delete;
    public:
        QString method;
        QJsonObject object;
        HandlerFunction handlerFunction;
        QMap<QByteArray, QByteArray> rawHeader;
        CallbackFunction successCallback;
        JSONCallbackFunction jsonSuccessCallback;
        CallbackFunction errorCallback;
        QString trackerUuid;
        RequestData():handlerFunction(0),
            successCallback(0),
            jsonSuccessCallback(0),
            errorCallback(0),
            trackerUuid(ExtUuid::generateUuid()) {}
        void addLoginToken(const QString &loginToken)
        {
            rawHeader["YACAPP-LoginToken"] = loginToken.toLatin1();
        }
        void addLoginEMail(const QString &loginEMail)
        {
            object["loginEMail"] = loginEMail;
            rawHeader["YACAPP-LoginEMail"] = loginEMail.toLatin1();
        }
        void addAppId(const QString appId)
        {
            rawHeader["YACAPP-AppId"] = appId.toLatin1();
        }
    };
protected:
    void post(const RequestData &rd);
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
    void setThirdMandant(const QString &third,
                         const QString &mandant);

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
