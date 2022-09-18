#include "menueconfig.h"
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

MenueConfig::MenueConfig(QObject *parent)
    : QObject{parent}
{

}

void MenueConfig::setConfig(const QJsonValue &config,
                            const QString &baseUrl)
{
    if (config["filename"].toString().length())
    {
        setFilename(config["filename"].toString());
        QFile jsonFile(baseUrl + config["filename"].toString());
        jsonFile.open(QIODevice::ReadOnly);
        QByteArray fileData(jsonFile.readAll());
        setConfig(QJsonDocument::fromJson(fileData)["menue"], baseUrl);
        return;
    }
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
    stringToJSON(filename);
    return config;
}
