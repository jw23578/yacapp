#ifndef YACNETWORK_H
#define YACNETWORK_H

#include <QNetworkAccessManager>

class YACNetwork: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    struct SRunningRequest;
    typedef std::function<void(QNetworkReply*, SRunningRequest &)> HandlerFunction;
    struct SRunningRequest
    {
        QString projectFilename;
        QString projectPackage;
        HandlerFunction handlerFunction;
        std::function<void()> successCallback;
        std::function<void(const QString &erroMessage)> errorCallback;
    };

    QMap<QNetworkReply*, SRunningRequest> runningRequests;

    void projectFilenameFinished(QNetworkReply *finishedReply, SRunningRequest &rr);
    void projectPackageFinished(QNetworkReply *finishedReply, SRunningRequest &rr);
public:
    YACNetwork();
    void downloadApp(QString projectFilename,
                     QString projectPackage,
                     std::function<void()> appDownloadedCallback,
                     std::function<void (const QString &)> errorCallback);

public slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // YACNETWORK_H
