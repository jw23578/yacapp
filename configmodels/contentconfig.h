#ifndef CONTENTCONFIG_H
#define CONTENTCONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include "contentitem.h"
#include <QQmlListProperty>
#include "configinterface.h"


class ContentConfig : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTYWITHOPTIONS(QString, type, Type, "", "webview", "column", "row");
    YACAPPPROPERTY(QString, url, Url, "");
    YACAPPPROPERTY(int, itemCount, ItemCount, 0)
    YACOBJECTLISTPROPERTY(ContentItem, item, Item, ContentConfig)
public:
    explicit ContentConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);
    QJsonObject getConfig();

signals:

};

#endif // CONTENTCONFIG_H
