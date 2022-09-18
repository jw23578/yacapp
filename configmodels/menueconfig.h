#ifndef MENUECONFIG_H
#define MENUECONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include <QList>
#include "menueitem.h"
#include <QQmlListProperty>

class MenueConfig : public QObject
{
    Q_OBJECT
    YACAPPPROPERTYWITHOPTIONS(QString, type, Type, "", "default", "+ Button");
    YACAPPPROPERTY(int, itemCount, ItemCount, 0);
    YACOBJECTLISTPROPERTY(MenueItem, item, Item, MenueConfig)
public:
    explicit MenueConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config,
                   const QString &baseUrl);

    QJsonObject getConfig();

    bool init(const QString &jsonConfigFile, const QString &baseUrl);
    void save(const QString &jsonConfigFile, const QString &baseUrl);
signals:

};

#endif // MENUECONFIG_H
