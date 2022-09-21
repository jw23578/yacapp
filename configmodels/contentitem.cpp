#include "contentitem.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QQmlEngine>

ContentItem::ContentItem(QObject *parent)
    : QObject{parent}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

void ContentItem::setConfig(const QJsonValue &config)
{
    setFilename(config["filename"].toString());
    setSize(config["size"].toDouble(0));
}

QJsonObject ContentItem::getConfig()
{
    QJsonObject config;
    toJSON(filename);
    toJSON(size);
    return config;
}
