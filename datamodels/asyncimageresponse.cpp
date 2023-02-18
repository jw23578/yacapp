#include "asyncimageresponse.h"

AsyncImageResponse::AsyncImageResponse():
    QQuickImageResponse()
{
}

AsyncImageResponse::AsyncImageResponse(const QString &pathToImage):
    QQuickImageResponse()
{
    if (pathToImage.size())
    {
        image.load(pathToImage);
    }
    emit finished();
}

void AsyncImageResponse::setImage(QImage image)
{
    this->image = image;
    emit finished();
}

QQuickTextureFactory *AsyncImageResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(image);
}
