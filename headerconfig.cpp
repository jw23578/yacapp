#include "headerconfig.h"
#include <QJsonValue>

HeaderConfig::HeaderConfig(QObject *parent)
    : QObject{parent}
{

}

void HeaderConfig::setConfig(const QJsonValue &config)
{
    setTitle(config["title"].toString());
    setHeight(config["height"].toDouble(0));
}
