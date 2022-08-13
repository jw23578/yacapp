#include "menueconfig.h"
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>

MenueConfig::MenueConfig(QObject *parent)
    : QObject{parent}
{

}

void MenueConfig::setConfig(const QJsonValue &config)
{
    if (config["filename"].toString().length())
    {
        QFile jsonFile(config["filename"].toString());
        jsonFile.open(QIODevice::ReadOnly);
        QByteArray fileData(jsonFile.readAll());
        setConfig(QJsonDocument::fromJson(fileData)["menue"]);
        return;
    }
    setType(config["type"].toString());
    QJsonArray array(config["items"].toArray());
    for (int i(0); i < array.size(); ++i)
    {
        MenueItem *item(new MenueItem);
        item->setConfig(array[i]);
        appendItem(item);
    }
}
