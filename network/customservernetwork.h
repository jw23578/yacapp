#ifndef CUSTOMSERVERNETWORK_H
#define CUSTOMSERVERNETWORK_H

#include "networkinterface.h"

class CustomServerNetwork : public NetworkInterface
{
    void projectFilenameFinished(QNetworkReply *finishedReply, SRunningRequest &rr);
    void projectPackageFinished(QNetworkReply *finishedReply, SRunningRequest &rr);
public:
    explicit CustomServerNetwork(QNetworkAccessManager &manager
                                 , const Constants &constants);

    void downloadApp(QString projectFilename,
                     QString projectPackage,
                     std::function<void(const QString &)> appDownloadedCallback,
                     std::function<void (const QString &)>  errorCallback);

};

#endif // CUSTOMSERVERNETWORK_H
