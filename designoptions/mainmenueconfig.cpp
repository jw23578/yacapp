#include "mainmenueconfig.h"
#include <QQmlEngine>
#include <QJsonObject>

MainMenueConfig::MainMenueConfig(QObject *parent)
    : ConfigInterface{parent}
{
}

void MainMenueConfig::setConfig(const QJsonValue &config)
{
    QJsonObject o(config.toObject());
    doubleFromJSONIfExists(smallElemCount, SmallElemCount);
    doubleFromJSONIfExists(smallElemHeightFactor, SmallElemHeightFactor);
    colorFromJSONIfExists(smallElemColor, SmallElemColor);
}

QJsonObject MainMenueConfig::getConfig() const
{
    QJsonObject config;
    doubleToJSON(smallElemCount);
    doubleToJSON(smallElemHeightFactor);
    colorToJSON(smallElemColor);
    return config;
}
