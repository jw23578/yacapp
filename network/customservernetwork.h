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

};

#endif // CUSTOMSERVERNETWORK_H
