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
    struct SRunningRequest
    {
        QString projectFilename;
        QString projectPackage;
        HandlerFunction handlerFunction;
        std::function<void(const QString &message)> successCallback;
        std::function<void(const QString &erroMessage)> errorCallback;
    };

    QMap<QNetworkReply*, SRunningRequest> runningRequests;

    void projectFilenameFinished(QNetworkReply *finishedReply, SRunningRequest &rr);
    void projectPackageFinished(QNetworkReply *finishedReply, SRunningRequest &rr);

    void registerUserFinished(QNetworkReply *finishedReply, SRunningRequest &rr);
public:
    YACNetwork(const Constants &constants);
    void downloadApp(QString projectFilename,
                     QString projectPackage,
                     std::function<void(const QString &)> appDownloadedCallback,
                     std::function<void (const QString &)>  errorCallback);
    void yacappServerRegisterUser(QString loginEMail,
                                  QString password,
                                  std::function<void (const QString &)> registerCallback,
                                  std::function<void (const QString &)>  errorCallback);
    void yacappServerVerifyUser(QString loginEMail,
                                QString verifyToken);
    void yacappServerLoginUser(QString loginEMail,
                               QString password);

    public slots:
        void replyFinished(QNetworkReply *reply);
};

#endif // YACNETWORK_H
