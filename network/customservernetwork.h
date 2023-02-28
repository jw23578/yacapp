#ifndef CUSTOMSERVERNETWORK_H
#define CUSTOMSERVERNETWORK_H

#include "networkinterface.h"

class CustomServerNetwork : public NetworkInterface
{
    void projectFilenameFinished(QNetworkReply *finishedReply, QByteArray &allData, SRunningRequest &rr);
    void projectPackageFinished(QNetworkReply *finishedReply, QByteArray &allData, SRunningRequest &rr);
public:
    explicit CustomServerNetwork(QNetworkAccessManager &manager
                                 , Constants &constants);

    void downloadApp(QString projectFilename,
                     QString projectPackage,
                     const QString &appId,
                     std::function<void(const QString &)> appDownloadedCallback,
                     std::function<void (const QString &)>  errorCallback);

};

#endif // CUSTOMSERVERNETWORK_H
