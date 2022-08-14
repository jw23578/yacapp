#include "headerconfig.h"
#include <QJsonValue>
#include <QJsonObject>

HeaderConfig::HeaderConfig(QObject *parent)
    : QObject{parent}
{

}

void HeaderConfig::setConfig(const QJsonValue &config)
{
    setTitle(config["title"].toString());
    setHeight(config["height"].toDouble(0));
}

QJsonObject HeaderConfig::getConfig()
{
    QJsonObject config;
    toJSON(title);
    toJSON(height);
    return config;
}
