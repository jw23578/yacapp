#include "menueconfig.h"
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

MenueConfig::MenueConfig(QObject *parent)
    : ConfigInterface{parent}
{

}

bool MenueConfig::init(const QString &jsonConfigFile)
{
    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument config = QJsonDocument::fromJson(fileData);
    setConfig(config["menue"]);
    return true;
}

void MenueConfig::setConfig(const QJsonValue &config)
{
    setType(config["type"].toString());
    clearItems();
    setItemCount(0);
    QJsonArray array(config["items"].toArray());
    for (int i(0); i < array.size(); ++i)
    {
        MenueItem *item(new MenueItem);
        item->setConfig(array[i]);
        appendItem(item);
    }
    setItemCount(m_items.length());
}

QJsonObject MenueConfig::getConfig()
{
    QJsonObject config;
    stringToJSON(type);
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

void MenueConfig::save(const QString &jsonConfigFile)
{
    QJsonObject config;
    config["menue"] = getConfig();

    QJsonObject::Iterator it(config.begin());
    while (it != config.end())
    {
        if (it.value() == QJsonObject())
        {
            it = config.erase(it);
        }
        else
        {
            ++it;
        }
    }

    QFile jsonFile(jsonConfigFile);
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(QJsonDocument(config).toJson());
}

void MenueConfig::addItem(int index)
{
    if (index < 0)
    {
        appendItem(new MenueItem);
        setItemCount(m_items.length());
        return;
    }
}

void MenueConfig::removeItem(int index)
{
    MenueItem *help(item(index));
    replaceItem(index, item(itemCount() - 1));
    removeLastItem();
    delete help;
    setItemCount(m_items.length());
}
