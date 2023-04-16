#include "headerconfig.h"
#include <QJsonValue>
#include <QJsonObject>

HeaderConfig::HeaderConfig(QObject *parent)
    : ConfigInterface{parent}
{

}

void HeaderConfig::setConfig(const QJsonValue &config)
{
    if (config.isUndefined())
    {
        return;
    }
    setColor(config["color"].toString());
    setTitle(config["title"].toString());
    setHeightPerThousand(config["heightPerThousand"].toDouble(0));
}

QJsonObject HeaderConfig::getConfig() const
{
    QJsonObject config;
    if (changed(color()) || changed(title()) || changed(heightPerThousand()))
    {
        colorToJSON(color);
        stringToJSON(title);
        toJSON(heightPerThousand);
    }
    return config;
}
