#include "footerconfig.h"
#include <QJsonValue>
#include <QJsonObject>

FooterConfig::FooterConfig(QObject *parent)
    : QObject{parent}
{

}

void FooterConfig::setConfig(const QJsonValue &config)
{
    setHeight(config["height"].toDouble(0));
}

QJsonObject FooterConfig::getConfig()
{
    QJsonObject config;
    toJSON(height);
    return config;
}
