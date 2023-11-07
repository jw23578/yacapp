#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include "constants.h"
#include <QNetworkReply>
#include "datamodels/transmissiontracker.h"

class NetworkInterface : public QObject
{
    Q_OBJECT
    TransmissionTracker finishedTracker;
    std::map<QString, TransmissionTracker*> uuid2TransmissionTracker;
    struct AcceptErrorsInfo
    {
        size_t currentCount;
        size_t maxCount;
    };

    std::map<QString, AcceptErrorsInfo> urlPart2errorInfo;
    bool acceptedError(const QString &url);
protected:
    QNetworkAccessManager &manager;
    Constants &constants;

    struct SRunningRequest;
    typedef std::function<void(QNetworkReply*, QByteArray &allData, SRunningRequest &)> HandlerFunction;
    typedef std::function<void(const QString &)> CallbackFunction;
    typedef std::function<void(const QJsonDocument &)> JSONCallbackFunction;
    struct SRunningRequest
    {
        QString projectFilename;
        QString projectPackage;
        QString appId;
        QString trackerUuid;
        HandlerFunction handlerFunction;
        CallbackFunction successCallback;
        JSONCallbackFunction jsonSuccessCallback;
        CallbackFunction errorCallback;
        NetworkInterface *networkInterface;
        SRunningRequest():handlerFunction(0), successCallback(0), jsonSuccessCallback(0), errorCallback(0), networkInterface(0) {}
    };
    friend void defaultReplyHandler(QNetworkReply *finishedReply, QByteArray &allData, NetworkInterface::SRunningRequest &rr);

private:
    QMap<QNetworkReply*, SRunningRequest> runningRequests;
protected:
    void addRunningRequest(QNetworkReply *reply, const SRunningRequest &rr);

public:
    explicit NetworkInterface(QNetworkAccessManager &manager, Constants &constants, QObject *parent = nullptr);
    std::function<void(const QString &message)> networkDefectCallback;
    std::function<void()> networkGoodCallback;

    TransmissionTracker *tracker(const QString &uuid);

    void acceptErrors(const QString &urlPart, size_t maxCount);

signals:
    void missingRight(int rightNumber);

public slots:
    void replyFinished(QNetworkReply *reply);
};

extern void defaultReplyHandler(QNetworkReply *finishedReply, QByteArray &allData, NetworkInterface::SRunningRequest &rr);


#endif // NETWORKINTERFACE_H
