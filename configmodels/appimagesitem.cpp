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
}

QJsonObject AppImagesItem::getConfig()
{
    QJsonObject config;
    stringToJSON(id);
    stringToJSON(fileUrl);
    return config;
}

void AppImagesItem::assign(const AppImagesItem &other)
{
    setId(other.id());
    setFileUrl(other.fileUrl());
}
