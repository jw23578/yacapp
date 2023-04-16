#ifndef MENUEITEM_H
#define MENUEITEM_H

#include <QObject>
#include "yacappmacros.h"
#include "configinterface.h"

class MenueItem : public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, caption, Caption, "")
    YACAPPPROPERTY(QString, target, Target, "")

public:
    explicit MenueItem(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config) override;

    QJsonObject getConfig() const override;
signals:

};

#endif // MENUEITEM_H
