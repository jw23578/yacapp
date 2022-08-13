#include "footerconfig.h"
#include <QJsonValue>

FooterConfig::FooterConfig(QObject *parent)
    : QObject{parent}
{

}

void FooterConfig::setConfig(const QJsonValue &config)
{
    setHeight(config["height"].toDouble(0));
}
