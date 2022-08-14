#include "contentitem.h"
#include <QJsonValue>

ContentItem::ContentItem(QObject *parent)
    : QObject{parent}
{

}

void ContentItem::setConfig(const QJsonValue &config)
{
    setFilename(config["filename"].toString());
    setHeight(config["height"].toDouble(0));
}
