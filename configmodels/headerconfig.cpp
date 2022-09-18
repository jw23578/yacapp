#include "headerconfig.h"
#include <QJsonValue>
#include <QJsonObject>

HeaderConfig::HeaderConfig(QObject *parent)
    : ConfigInterface{parent}
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
    if (changed(color()) || changed(title()) || changed(height()))
    {
        colorToJSON(color);
        stringToJSON(title);
        toJSON(height);
    }
    return config;
}
