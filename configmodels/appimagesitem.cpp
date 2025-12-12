#include "appimagesitem.h"
#include <QJsonValue>
#include <QJsonObject>

AppImagesItem::AppImagesItem(QObject *parent)
    : ConfigInterface{parent}
{

}

void AppImagesItem::setConfig(const QJsonValue &config)
{
    if (config.isUndefined())
    {
        return;
    }
    stringFromJSON(id, Id);
    stringFromJSON(fileUrl, FileUrl);
    stringFromJSON(imageId, ImageId);
    stringFromJSON(largeobject2objectId, Largeobject2objectId);
}

QJsonObject AppImagesItem::getConfig() const
{
    QJsonObject config;
    stringToJSON(id);
    stringToJSON(fileUrl);
    stringToJSON(imageId);
    stringToJSON(largeobject2objectId);
    return config;
}

void AppImagesItem::assign(const AppImagesItem &other)
{
    setId(other.id());
    setFileUrl(other.fileUrl());
}
