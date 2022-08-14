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
    YACAPPPROPERTY(QString, type, Type, "")
    YACAPPPROPERTY(int, itemCount, ItemCount, 0);
    YACOBJECTLISTPROPERTY(MenueItem, item, Item, MenueConfig)
public:
    explicit MenueConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config,
                   const QString &baseUrl);

signals:

};

#endif // MENUECONFIG_H
