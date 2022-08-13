#include "menueitem.h"
#include <QJsonValue>

MenueItem::MenueItem(QObject *parent)
    : QObject{parent}
{

}

void MenueItem::setConfig(const QJsonValue &config)
{
    setCaption(config["caption"].toString());
    setTargetFile(config["targetFile"].toString());
    setSpecialMeaning(config["specialMeaning"].toString());
}
