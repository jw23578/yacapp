#ifndef ASYNCIMAGERESPONSE_H
#define ASYNCIMAGERESPONSE_H

#include <QQuickAsyncImageProvider>

class AsyncImageResponse : public QQuickImageResponse
{

    QImage image;
public:
    AsyncImageResponse();
    AsyncImageResponse(QString const &pathToImage);

    void setImage(QImage image);

    QQuickTextureFactory *textureFactory() const override;

};
#endif // ASYNCIMAGERESPONSE_H
