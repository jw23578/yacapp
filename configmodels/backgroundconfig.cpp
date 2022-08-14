#include "backgroundconfig.h"
#include <QJsonValue>
#include <QJsonObject>

BackgroundConfig::BackgroundConfig(QObject *parent)
{

}

void BackgroundConfig::setConfig(const QJsonValue &config)
{
    setColor(config["color"].toString());
    setImageFilename(config["imageFilename"].toString());
}

QJsonObject BackgroundConfig::getConfig()
{
    QJsonObject config;
    colorToJSON(color);
    toJSON(imageFilename);
    return config;
}
