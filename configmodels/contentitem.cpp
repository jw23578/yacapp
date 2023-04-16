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
    stringFromJSON(fillModeString, FillModeString);
    doubleFromJSON(widthPerThousand, WidthPerThousand);
    doubleFromJSON(heightPerThousand, HeightPerThousand);
    intFromJSON(leftMarginPerThousand, LeftMarginPerThousand);
    intFromJSON(topMarginPerThousand, TopMarginPerThousand);
    intFromJSON(bottomMarginPerThousand, BottomMarginPerThousand);
    intFromJSON(rightMarginPerThousand, RightMarginPerThousand);
    stringFromJSON(text, Text);
    colorFromJSON(color, Color);
    stringFromJSON(text2, Text2);
    colorFromJSON(color2, Color2);
    stringFromJSON(flipableFilename, FlipableFilename);
    stringFromJSON(flipableFilename2, FlipableFilename2);
}

QJsonObject ContentItem::getConfig() const
{
    QJsonObject config;
    stringToJSON(filename);
    toJSON(type);
    stringToJSON(target);
    stringToJSON(url);
    stringToJSON(fillModeString);
    toJSON(widthPerThousand);
    toJSON(heightPerThousand);
    toJSON(leftMarginPerThousand);
    toJSON(rightMarginPerThousand);
    toJSON(topMarginPerThousand);
    toJSON(bottomMarginPerThousand);
    toJSON(text);
    colorToJSON(color);
    toJSON(text2);
    colorToJSON(color2);
    toJSON(flipableFilename);
    toJSON(flipableFilename2);
    return config;
}
