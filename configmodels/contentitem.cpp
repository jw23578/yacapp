#include "contentitem.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QQmlEngine>

ContentItem::ContentItem(QObject *parent)
    : ConfigInterface{parent}
{
}

void ContentItem::setConfig(const QJsonValue &config)
{
    stringFromJSON(filename, Filename);
    stringFromJSON(type, Type);
    setType("file");
    stringFromJSON(target, Target);
    stringFromJSON(url, Url);
    doubleFromJSON(width, Width);
    doubleFromJSON(height, Height);
}

QJsonObject ContentItem::getConfig()
{
    QJsonObject config;
    stringToJSON(filename);
    toJSON(type);
    stringToJSON(target);
    stringToJSON(url);
    toJSON(width);
    toJSON(height);
    return config;
}
