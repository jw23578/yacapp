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
}

QJsonObject AppImagesItem::getConfig() const
{
    QJsonObject config;
    stringToJSON(id);
    stringToJSON(fileUrl);
    stringToJSON(imageId);
    return config;
}

void AppImagesItem::assign(const AppImagesItem &other)
{
    setId(other.id());
    setFileUrl(other.fileUrl());
}
