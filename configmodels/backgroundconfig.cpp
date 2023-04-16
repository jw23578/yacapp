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
    colorFromJSON(color, Color)
    stringFromJSON(imageFilename, ImageFilename)
}

QJsonObject BackgroundConfig::getConfig() const
{
    QJsonObject config;
    if (changed(color()) || changed(imageFilename()))
    {
        colorToJSON(color);
        stringToJSON(imageFilename);
    }
    return config;
}
