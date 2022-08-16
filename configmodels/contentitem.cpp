#include "contentitem.h"
#include <QJsonValue>
#include <QJsonObject>

ContentItem::ContentItem(QObject *parent)
    : QObject{parent}
{

}

void ContentItem::setConfig(const QJsonValue &config)
{
    setFilename(config["filename"].toString());
    setHeight(config["height"].toDouble(0));
}

QJsonObject ContentItem::getConfig()
{
    QJsonObject config;
    toJSON(filename);
    toJSON(height);
    return config;
}
