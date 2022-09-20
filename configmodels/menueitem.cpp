#include "menueitem.h"
#include <QJsonValue>
#include <QJsonObject>

MenueItem::MenueItem(QObject *parent)
    : ConfigInterface{parent}
{

}

void MenueItem::setConfig(const QJsonValue &config)
{
    setCaption(config["caption"].toString());
    setTargetFile(config["targetFile"].toString());
    setSpecialMeaning(config["specialMeaning"].toString());
}

QJsonObject MenueItem::getConfig()
{
    QJsonObject config;
    toJSON(caption);
    toJSON(targetFile);
    toJSON(specialMeaning);
    return config;
}

