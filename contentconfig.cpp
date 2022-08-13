#include "contentconfig.h"
#include <QJsonValue>
#include <QJsonArray>

ContentConfig::ContentConfig(QObject *parent)
    : QObject{parent}
{

}

void ContentConfig::setConfig(const QJsonValue &config)
{
    setType(config["type"].toString());
    setUrl(config["url"].toString());
    QJsonArray array(config["items"].toArray());
    for (int i(0); i < array.count(); ++i)
    {
        ContentItem *item(new ContentItem);
        item->setConfig(array[i]);
        appendItem(item);
    }
}
