#ifndef YACNETWORK_H
#define YACNETWORK_H

#include <QNetworkAccessManager>
#include "constants.h"

class YACNetwork: public QObject
{
    Q_OBJECT
    const QString yacappServerUrl = {"http://127.0.0.1:23578"};
    const Constants &constants;
    QNetworkAccessManager manager;
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

    void projectFilenameFinished(QNetworkReply *finishedReply, SRunningRequest &rr);
    void projectPackageFinished(QNetworkReply *finishedReply, SRunningRequest &rr);

    void registerUserFinished(QNetworkReply *finishedReply, SRunningRequest &rr);
    void verifyUserFinished(QNetworkReply *finishedReply, SRunningRequest &rr);
    void loginUserFinished(QNetworkReply *finishedReply, SRunningRequest &rr);

    void yacappServerPost(const QString &method,
                          const QJsonObject &object,
                          HandlerFunction handlerFunction,
                          CallbackFunction registerCallback,
                          CallbackFunction errorCallback);
public:
    YACNetwork(const Constants &constants);
    void downloadApp(QString projectFilename,
                     QString projectPackage,
                     std::function<void(const QString &)> appDownloadedCallback,
                     std::function<void (const QString &)>  errorCallback);
    void yacappServerRegisterUser(QString loginEMail,
                                  QString password,
                                  CallbackFunction registerCallback,
                                  CallbackFunction  errorCallback);
    void yacappServerVerifyUser(QString loginEMail,
                                QString verifyToken,
                                CallbackFunction registerCallback,
                                CallbackFunction  errorCallback);
    void yacappServerLoginUser(QString loginEMail,
                               QString password,
                               CallbackFunction registerCallback,
                               CallbackFunction  errorCallback);

    public slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // YACNETWORK_H
