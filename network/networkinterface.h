#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include "constants.h"
#include <QNetworkReply>

class NetworkInterface : public QObject
{
    Q_OBJECT
protected:
    QNetworkAccessManager &manager;
    const Constants &constants;


    struct SRunningRequest;
    typedef std::function<void(QNetworkReply*, SRunningRequest &)> HandlerFunction;
    typedef std::function<void(const QString &)> CallbackFunction;
    typedef std::function<void(const QJsonDocument &)> JSONCallbackFunction;
    struct SRunningRequest
    {
        QString projectFilename;
        QString projectPackage;
        HandlerFunction handlerFunction;
        CallbackFunction successCallback;
        JSONCallbackFunction jsonSuccessCallback;
        CallbackFunction errorCallback;
        SRunningRequest():handlerFunction(0), successCallback(0), jsonSuccessCallback(0), errorCallback(0) {}
    };
    friend void defaultJSONReplyHandler(QNetworkReply *finishedReply, SRunningRequest &rr);
    friend void defaultReplyHandler(QNetworkReply *finishedReply, NetworkInterface::SRunningRequest &rr);

    QMap<QNetworkReply*, SRunningRequest> runningRequests;

public:
    explicit NetworkInterface(QNetworkAccessManager &manager, const Constants &constants, QObject *parent = nullptr);

signals:

public slots:
    void replyFinished(QNetworkReply *reply);
};

extern void defaultReplyHandler(QNetworkReply *finishedReply, NetworkInterface::SRunningRequest &rr);
extern void defaultJSONReplyHandler(QNetworkReply *finishedReply, NetworkInterface::SRunningRequest &rr);


#endif // NETWORKINTERFACE_H
