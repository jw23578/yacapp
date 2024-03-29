#ifndef HEADERCONFIG_H
#define HEADERCONFIG_H

#include <QObject>
#include "yacappmacros.h"
#include <QColor>
#include "configinterface.h"

class HeaderConfig : public ConfigInterface
{
    Q_OBJECT

    YACAPPPROPERTY(QString, title, Title, "")
    YACAPPPROPERTY(double, heightPerThousand, HeightPerThousand, 0)
    YACAPPPROPERTY(QColor, color, Color, "white")
public:
    explicit HeaderConfig(QObject *parent = nullptr);

    void setConfig(QJsonValue const &config) override;
    QJsonObject getConfig() const override;
signals:

};

#endif // HEADERCONFIG_H
