#include "splashscreenconfig.h"
#include <QJsonValue>
#include <QJsonObject>

SplashscreenConfig::SplashscreenConfig(QObject *parent)
    : ConfigInterface{parent}
{

}

void SplashscreenConfig::setConfig(const QJsonValue &config)
{
    setImageFilename(config["imageFilename"].toString());
    setSecondsDuration(config["secondsDuration"].toInt());
}

QJsonObject SplashscreenConfig::getConfig()
{
    QJsonObject config;
    if (changed(imageFilename()) || changed(secondsDuration()))
    {
        stringToJSON(imageFilename);
        toJSON(secondsDuration);
    }
    return config;
}
