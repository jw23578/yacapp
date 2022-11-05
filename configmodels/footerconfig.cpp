#include "footerconfig.h"
#include <QJsonValue>
#include <QJsonObject>

FooterConfig::FooterConfig(QObject *parent)
    : ConfigInterface{parent}
{

}

void FooterConfig::setConfig(const QJsonValue &config)
{
    if (config.isUndefined())
    {
        return;
    }
    setHeightPerThousand(config["heightPerThousand"].toDouble(0));
    setColor(config["color"].toString());
}

QJsonObject FooterConfig::getConfig()
{
    QJsonObject config;
    if (changed(heightPerThousand()) || changed(color()))
    {
        toJSON(heightPerThousand);
        colorToJSON(color);
    }
    return config;
}
