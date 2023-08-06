#include "asyncimageprovider.h"
#include <QFile>
#include "yacapp.h"

AsyncImageProvider::AsyncImageProvider(YACAPP &yacApp,
                                       YACServerNetwork &yacServerNetwork):
    QQuickAsyncImageProvider(),
    yacApp(yacApp),
    yacServerNetwork(yacServerNetwork)
{
}

QQuickImageResponse *AsyncImageProvider::requestImageResponse(const QString &id,
                                                              const QSize &requestedSize)
{
    const std::lock_guard<std::mutex> guard(mutex);
    std::string type;
    QStringList parts(id.split("/"));
    if (parts.size() < 2)
    {
        // FIXME Error Logging?
        return new AsyncImageResponse("");
    }
    QString imageType(parts[0]);
    if (imageType == "profileImage" || imageType == "appImage")
    {
        if (parts[1].size() == 0)
        {
            return new AsyncImageResponse("");
        }
        QString uuid(parts[1]);
        QString cachename(uuid + "#" + QString::number(requestedSize.width()) + "x" + QString::number(requestedSize.height()));
        QString cacheFileName(yacApp.constants.getCachePath() + cachename + ".jpg");
        if (QFile::exists(cacheFileName))
        {
            return new AsyncImageResponse(cacheFileName);
        }
        AsyncImageResponse *air(new AsyncImageResponse);
        yacApp.addFileToFetch(imageType,
                              uuid,
                              air,
                              cacheFileName);
        return air;
    }
    return new AsyncImageResponse("");
    /*
 *  ExtString::leftRight(id.toStdString(), ".", type, uuid);
    if (uuid == NullUuid.str())
    {
        return new AsyncImageResponse("");
    }
    std::string cachename(uuid + "#" + ExtString::toString(requestedSize.width()) + "x" + ExtString::toString(requestedSize.height()));
    QString cacheFileName(jw78::Utils::getCachePath() + "/" + cachename.c_str() + ".jpg");
    if (std::filesystem::exists(cacheFileName.toStdString()))
    {
        return new AsyncImageResponse(cacheFileName);
    }

    AsyncImageResponse *response = new AsyncImageResponse;

    id2response[uuid] = response;
    id2cachename[uuid] = cachename;
    if (type == "file")
    {
        netwworkBusinessLogic.addFileToFetch(uuid, requestedSize.width(), requestedSize.height(),
                                             std::bind(&NexusImageProvider::fetchFileHandler, this, std::placeholders::_1, std::placeholders::_2));
    }
    return response;*/

}

void AsyncImageProvider::imageLoaded(const std::string &imageDataBase64, const std::string &uuid)
{
    /*    const std::lock_guard<std::mutex> guard(mutex);
    auto it(id2response.find(uuid));
    if (it == id2response.end())
    {
        return;
    }
    QImage image;
    image.loadFromData(QByteArray::fromBase64(imageDataBase64.c_str()));
    it->second->setImage(image);
    QString cacheFileName(jw78::Utils::getCachePath() + "/" + id2cachename[uuid].c_str() + ".jpg");
    image.save(cacheFileName);
    id2response.erase(uuid);
    id2cachename.erase(uuid); */
}
