#ifndef CONTENTITEM_H
#define CONTENTITEM_H

#include <QObject>
#include "yacappmacros.h"
#include "configinterface.h"

class ContentItem : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTYWITHOPTIONS(QString, type, Type, "", "file", "webview", "image");
    YACAPPPROPERTY(QString, filename, Filename, "")
    YACAPPPROPERTY(QString, url, Url, "")
    YACAPPPROPERTY(QString, target, Target, "")
    YACAPPPROPERTY(double, width, Width, 0)
    YACAPPPROPERTY(double, height, Height, 0)
public:
    explicit ContentItem(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config) override;
    QJsonObject getConfig() override;

signals:

};

#endif // CONTENTITEM_H
