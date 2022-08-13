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
    YACOBJECTLISTPROPERTY(MenueItem, item, Item, MenueConfig)
public:
    explicit MenueConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config);

signals:

};

#endif // MENUECONFIG_H
