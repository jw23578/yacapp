#include "contentconfig.h"
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

ContentConfig::ContentConfig(QObject *parent)
    : ConfigInterface{parent}
{

}

void ContentConfig::setConfig(const QJsonValue &config)
{
    if (config.isUndefined())
    {
        return;
    }
    setType(config["type"].toString());
    setUrl(config["url"].toString());
    intFromJSON(columns, Columns);
    intFromJSON(rows, Rows);
    boolFromJSON(loginNeeded, LoginNeeded);
    QJsonArray array(config["items"].toArray());
    setItemCount(0);
    for (int i(0); i < array.count(); ++i)
    {
        ContentItem *item(new ContentItem);
        item->setConfig(array[i]);
        appendItem(item);
    }
    setItemCount(m_items.length());
}

QJsonObject ContentConfig::getConfig()
{
    QJsonObject config;
    stringToJSON(type);
    stringToJSON(url);
    intToJSON(columns);
    intToJSON(rows);
    boolToJSON(loginNeeded);
    if (itemCount())
    {
        QJsonArray itemArray;
        for (int i(0); i < itemCount(); ++i)
        {
            itemArray.append(item(i)->getConfig());
        }
        config["items"] = itemArray;
    }
    return config;
}

void ContentConfig::addItem(int index)
{
    if (index < 0)
    {
        appendItem(new ContentItem);
        setItemCount(m_items.length());
        return;
    }
}

void ContentConfig::removeItem(int index)
{
    ContentItem *help(item(index));
    replaceItem(index, item(itemCount() - 1));
    removeLastItem();
    delete help;
    setItemCount(m_items.length());
}
