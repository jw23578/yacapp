#ifndef CONTENTCONFIG_H
#define CONTENTCONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include "contentitem.h"
#include <QQmlListProperty>


class ContentConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTYWITHOPTIONS(QString, type, Type, "", "webview", "column");
    YACAPPPROPERTY(QString, url, Url, "");
    YACAPPPROPERTY(int, itemCount, ItemCount, 0)
    YACOBJECTLISTPROPERTY(ContentItem, item, Item, ContentConfig)
public:
    explicit ContentConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);

signals:

};

#endif // CONTENTCONFIG_H
