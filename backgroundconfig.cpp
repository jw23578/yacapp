#include "backgroundconfig.h"
#include <QJsonValue>

BackgroundConfig::BackgroundConfig(QObject *parent)
{

}

void BackgroundConfig::setConfig(const QJsonValue &config)
{
    setColor(config["color"].toString());
    setImageFilename(config["imageFilename"].toString());
}
