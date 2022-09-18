#include "headerconfig.h"
#include <QJsonValue>
#include <QJsonObject>

HeaderConfig::HeaderConfig(QObject *parent)
    : QObject{parent}
{

}

void HeaderConfig::setConfig(const QJsonValue &config)
{
    setColor(config["color"].toString());
    setTitle(config["title"].toString());
    setHeight(config["height"].toDouble(0));
}

QJsonObject HeaderConfig::getConfig()
{
    QJsonObject config;
    colorToJSON(color);
    stringToJSON(title);
    toJSON(height);
    return config;
}
