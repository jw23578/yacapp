#include "globaldesignconfig.h"
#include <QJsonObject>
#include <QJsonValue>

GlobalDesignConfig::GlobalDesignConfig(QObject *parent)
    : ConfigInterface{parent}
{

}

void GlobalDesignConfig::setConfig(const QJsonValue &config)
{
    QJsonObject o(config.toObject());
    if (o.contains("defaultFontPointSize"))
    {
        int value(o["defaultFontPointSize"].toInt());
        if (value)
        {
            setDefaultFontPointSize(value);
        }
    }
}

QJsonObject GlobalDesignConfig::getConfig()
{
    QJsonObject config;
    intToJSON(defaultFontPointSize);
    return config;
}
