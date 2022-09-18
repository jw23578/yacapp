#include "splashscreenconfig.h"
#include <QJsonValue>
#include <QJsonObject>

SplashscreenConfig::SplashscreenConfig(QObject *parent)
    : QObject{parent}
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
    toJSON(imageFilename);
    toJSON(secondsDuration);
    return config;
}
