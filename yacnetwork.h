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
    YACNetwork(const Constants &constants);
    void downloadApp(QString projectFilename,
                     QString projectPackage,
                     std::function<void(const QString &)> appDownloadedCallback,
                     std::function<void (const QString &)>  errorCallback);
    void yacappServerRegisterUser(QString loginEMail,
                                  QString password,
                                  CallbackFunction successCallback,
                                  CallbackFunction  errorCallback);
    void yacappServerVerifyUser(QString loginEMail,
                                QString verifyToken,
                                CallbackFunction successCallback,
                                CallbackFunction  errorCallback);
    void yacappServerLoginUser(QString loginEMail,
                               QString password,
                               CallbackFunction successCallback,
                               CallbackFunction  errorCallback);
    void yacappServerUserLoggedIn(QString loginEMail,
                                  QString verifyToken,
                                  CallbackFunction successCallback,
                                  CallbackFunction  errorCallback);

    void yacappServerUploadApp(const QString &loginEMail,
                               const QString &loginToken,
                               const QString &app_id,
                               const QString &app_name,
                               const QString &app_logo_url,
                               const QString &app_color_name,
                               const QString &json_yacapp,
                               const QString &yacpck_base64,
                               CallbackFunction successCallback,
                               CallbackFunction  errorCallback);

    void yacappServerGetAllAPPs(CallbackFunction successCallback,
                                CallbackFunction  errorCallback);

public slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // YACNETWORK_H
