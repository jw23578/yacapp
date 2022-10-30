#include "backgroundconfig.h"
#include <QJsonValue>
#include <QJsonObject>

BackgroundConfig::BackgroundConfig(QObject *parent):
    ConfigInterface(parent)
{

}

void BackgroundConfig::setConfig(const QJsonValue &config)
{
    if (config.isUndefined())
    {
        return;
    }
    setColor(config["color"].toString());
    setImageFilename(config["imageFilename"].toString());
}

QJsonObject BackgroundConfig::getConfig()
{
    QJsonObject config;
    if (changed(color()) || changed(imageFilename()))
    {
        colorToJSON(color);
        stringToJSON(imageFilename);
    }
    return config;
}
