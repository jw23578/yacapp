#ifndef MENUECONFIG_H
#define MENUECONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include <QList>
#include "menueitem.h"
#include <QQmlListProperty>
#include "configinterface.h"

class MenueConfig : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTYWITHOPTIONS(QString, type, Type, "", "default", "+ Button");
    YACAPPPROPERTY(int, itemCount, ItemCount, 0);
    YACOBJECTLISTPROPERTY(MenueItem, item, Item, MenueConfig)
public:
    explicit MenueConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config) override;

    QJsonObject getConfig() const override;

    bool init(const QString &jsonConfigFile);
    void save(const QString &jsonConfigFile);

    Q_INVOKABLE void addItem(int index);
    Q_INVOKABLE void removeItem(int index);

signals:

};

#endif // MENUECONFIG_H
