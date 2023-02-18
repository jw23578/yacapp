#ifndef ASYNCIMAGEPROVIDER_H
#define ASYNCIMAGEPROVIDER_H

#include <QQuickAsyncImageProvider>
#include "yacservernetwork.h"
#include <mutex>
#include "datamodels/asyncimageresponse.h"

class YACAPP;

class AsyncImageProvider : public QQuickAsyncImageProvider
{
    YACAPP &yacApp;
    YACServerNetwork &yacServerNetwork;
    std::mutex mutex;
    std::map<std::string, AsyncImageResponse*> id2response;
    std::map<std::string, std::string> id2cachename;
public:
    AsyncImageProvider(YACAPP &yacApp,
                       YACServerNetwork &yacServerNetwork);

    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

    void imageLoaded(const std::string &imageDataBase64,
                     const std::string &uuid);
};

#endif // ASYNCIMAGEPROVIDER_H
