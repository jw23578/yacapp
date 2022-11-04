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
    struct SRunningRequest
    {
        QString projectFilename;
        QString projectPackage;
        HandlerFunction handlerFunction;
        CallbackFunction successCallback;
        CallbackFunction errorCallback;
    };

    QMap<QNetworkReply*, SRunningRequest> runningRequests;

public:
    explicit NetworkInterface(QNetworkAccessManager &manager, const Constants &constants, QObject *parent = nullptr);

signals:

public slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // NETWORKINTERFACE_H
