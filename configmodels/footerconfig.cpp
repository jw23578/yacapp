#include "footerconfig.h"
#include <QJsonValue>
#include <QJsonObject>

FooterConfig::FooterConfig(QObject *parent)
    : ConfigInterface{parent}
{

}

void FooterConfig::setConfig(const QJsonValue &config)
{
    setHeight(config["height"].toDouble(0));
    setColor(config["color"].toString());
}

QJsonObject FooterConfig::getConfig()
{
    QJsonObject config;
    if (changed(height()) || changed(color()))
    {
        toJSON(height);
        colorToJSON(color);
    }
    return config;
}
