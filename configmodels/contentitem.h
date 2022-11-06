#ifndef CONTENTITEM_H
#define CONTENTITEM_H

#include <QObject>
#include "yacappmacros.h"
#include "configinterface.h"
#include <QColor>

class ContentItem : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTYWITHOPTIONS(QString, type, Type, "", "file", "webview", "image", "tile", "slidertile");
    YACAPPPROPERTY(QString, filename, Filename, "")
    YACAPPPROPERTY(QString, url, Url, "")
    YACAPPPROPERTY(QString, target, Target, "")
    YACAPPPROPERTY(double, widthPerThousand, WidthPerThousand, 0)
    YACAPPPROPERTY(double, heightPerThousand, HeightPerThousand, 0)
    YACAPPPROPERTY(QString, text, Text, "")
    YACAPPPROPERTY(QColor, color, Color, "white")
    YACAPPPROPERTY(QString, text2, Text2, "")
    YACAPPPROPERTY(QColor, color2, Color2, "white")

public:
    explicit ContentItem(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config) override;
    QJsonObject getConfig() override;

signals:

};

#endif // CONTENTITEM_H
