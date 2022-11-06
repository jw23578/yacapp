#include "contentitem.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QQmlEngine>

ContentItem::ContentItem(QObject *parent)
    : ConfigInterface{parent}
{
}

void ContentItem::setConfig(const QJsonValue &config)
{
    stringFromJSON(filename, Filename);
    stringFromJSON(type, Type);
    stringFromJSON(target, Target);
    stringFromJSON(url, Url);
    doubleFromJSON(widthPerThousand, WidthPerThousand);
    doubleFromJSON(heightPerThousand, HeightPerThousand);
    stringFromJSON(text, Text);
    colorFromJSON(color, Color);
    stringFromJSON(text2, Text2);
    colorFromJSON(color2, Color2);
    stringFromJSON(flipableFilename, FlipableFilename);
    stringFromJSON(flipableFilename2, FlipableFilename2);
}

QJsonObject ContentItem::getConfig()
{
    QJsonObject config;
    stringToJSON(filename);
    toJSON(type);
    stringToJSON(target);
    stringToJSON(url);
    toJSON(widthPerThousand);
    toJSON(heightPerThousand);
    toJSON(text);
    colorToJSON(color);
    toJSON(text2);
    colorToJSON(color2);
    toJSON(flipableFilename);
    toJSON(flipableFilename2);
    return config;
}
