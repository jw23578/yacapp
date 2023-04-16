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
    stringFromJSON(target, Target)
}

QJsonObject MenueItem::getConfig() const
{
    QJsonObject config;
    toJSON(caption);
    stringToJSON(target);
    return config;
}

